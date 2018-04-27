#include "stdafx.h"
#include "ProcessTabWidget.h"
#include "ModuleDlg.h"
#include "ThreadDlg.h"
#include "Data.h"
#include "MainWindow.h"
#include <cstdlib>
#include <TlHelp32.h>

ProcessTabWidget::ProcessTabWidget(QWidget *parent)
	: QWidget(parent), m_nProcessCnt(0)
{
	ui.setupUi(this);

	connect(this, &ProcessTabWidget::updateProcessNum, (SohoSecurity*)parent, &SohoSecurity::onUpdateProcessNum);

	QStringList headers;
	headers << "映像名称" << "PID" << "PPID" << "路径";
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	ui.tableWidget->setColumnWidth(0, ui.tableWidget->width() / 8 * 2);
	ui.tableWidget->setColumnWidth(1, ui.tableWidget->width() / 8);
	ui.tableWidget->setColumnWidth(2, ui.tableWidget->width() / 8);
	ui.tableWidget->setColumnWidth(3, ui.tableWidget->width() / 2);

	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	listProcess();
}

ProcessTabWidget::~ProcessTabWidget()
{
}

void ProcessTabWidget::onTableWidgetCustomContextMenuRequest(QPoint pos)
{
	QTableWidgetItem *pCurItem = ui.tableWidget->itemAt(pos);
	if (pCurItem == nullptr)
		return;

	QTableWidgetItem *pPIDItem = ui.tableWidget->selectedItems().at(1);
	if (pPIDItem->text().toULong() == 0)
		return;

	QMenu *pPopMenu = new QMenu(ui.tableWidget);
	QAction *pActRefresh = pPopMenu->addAction("刷新");
	QAction *pActTraverseModule = pPopMenu->addAction("查看进程模块");
	QAction *pActTraverseThread = pPopMenu->addAction("查看进程线程");
	QAction *pActEndProcess = pPopMenu->addAction("结束进程");

	connect(pActRefresh, &QAction::triggered, this, &ProcessTabWidget::onPopMenuRefresh);
	connect(pActTraverseModule, &QAction::triggered, this, &ProcessTabWidget::onPopMenuTraverseModule);
	connect(pActTraverseThread, &QAction::triggered, this, &ProcessTabWidget::onPopMenuTraverseThread);
	connect(pActEndProcess, &QAction::triggered, this, &ProcessTabWidget::onPopMenuEndProcess);

	pPopMenu->exec(QCursor::pos());
	
	delete pActRefresh; pActRefresh = nullptr;
	delete pActTraverseModule; pActTraverseModule = nullptr;
	delete pActTraverseThread; pActTraverseThread = nullptr;
	delete pPopMenu; pPopMenu = nullptr;
}

void ProcessTabWidget::onPopMenuRefresh()
{
	ui.tableWidget->clearContents();
	ui.tableWidget->setSortingEnabled(false); //->解决排序后刷新数据会造成控件上数据很多都为空白的bug
	listProcess();
	ui.tableWidget->setSortingEnabled(true);
}

void ProcessTabWidget::onPopMenuTraverseModule()
{
	QTableWidgetItem *pNameItem = ui.tableWidget->selectedItems().at(0);
	QTableWidgetItem *pPIDItem = ui.tableWidget->selectedItems().at(1);

	PROCESS2MODULEINFO p2mInfo = {};
	p2mInfo.dwPID = pPIDItem->text().toULong();
	p2mInfo.strPName = pNameItem->text();
	ModuleDlg moduleDlg(&p2mInfo, this);
	moduleDlg.exec();
}

void ProcessTabWidget::onPopMenuTraverseThread()
{
	QTableWidgetItem *pNameItem = ui.tableWidget->selectedItems().at(0);
	QTableWidgetItem *pPIDItem = ui.tableWidget->selectedItems().at(1);

	PROCESS2THREADINFO p2tInfo = {};
	p2tInfo.dwPID = pPIDItem->text().toULong();
	p2tInfo.strPName = pNameItem->text();
	ThreadDlg threadDlg(&p2tInfo, this);
	threadDlg.exec();
}

void ProcessTabWidget::onPopMenuEndProcess()
{
	QTableWidgetItem *pNameItem = ui.tableWidget->selectedItems().at(0);
	QTableWidgetItem *pPIDItem = ui.tableWidget->selectedItems().at(1);
	DWORD dwPID = pPIDItem->text().toULong();
	QString strPName = pNameItem->text();
	QMessageBox::StandardButton retBtn = QMessageBox::question(this, tr("询问"), tr("确定要结束 '%1' 吗?").arg(strPName));
	if (retBtn == QMessageBox::Yes) {
		//结束进程
		if (endProcess(dwPID)) {
			ui.tableWidget->removeRow(pNameItem->row());
		}
		else {
			QMessageBox::critical(this, tr("错误"), tr("结束进程失败"));
		}
	}
}

bool ProcessTabWidget::endProcess(DWORD dwPID)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, NULL, dwPID);
	if (hProcess)
		return TerminateProcess(hProcess, 0) != 0;
	else
		return false;
}

void ProcessTabWidget::listProcess()
{
	HANDLE hProcessSnap = NULL;
	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32 = {};
	UINT uPID;
	UINT uPPID;
	BOOL Is32 = FALSE;
	QString strProcessFullPath;

	int i = 0;

	if ((hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hProcessSnap, &pe32);

	if (!bRet) {
		//TODO:	
		CloseHandle(hProcessSnap);
		return;
	}
	else {
		do {
			if (pe32.th32ProcessID == GetCurrentProcessId()) {
				continue;
			}
			else if (pe32.th32ProcessID == 0) {
				strProcessFullPath = "处理器空闲时间的百分比";
			}
			else {
				//获取进程对应程序的路径
				getSeDebugPrivilge();
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
				if (hProcess) {
					IsWow64Process(hProcess, &Is32);
					TCHAR tFullPathBuf[MAX_PATH] = {};
					DWORD dwSize = MAX_PATH;
					if (QueryFullProcessImageName(hProcess, 0, tFullPathBuf, &dwSize))
						strProcessFullPath = QString::fromWCharArray(tFullPathBuf);
					else
						strProcessFullPath = "无法获取";
				}
				else {
					strProcessFullPath = "无法获取";
				}
			}

			ui.tableWidget->setRowCount(i+1);
			ui.tableWidget->setItem(i, 0, 
				new QTableWidgetItem(QString::fromWCharArray(pe32.szExeFile) + (Is32 ? " *32" : "")));
			QTableWidgetItem *pPIDItem = new QTableWidgetItem;
			pPIDItem->setTextAlignment(Qt::AlignCenter);
			uPID = pe32.th32ProcessID;
			pPIDItem->setData(Qt::DisplayRole, uPID);
			ui.tableWidget->setItem(i, 1, pPIDItem);
			QTableWidgetItem *pPPIDItem = new QTableWidgetItem;
			pPPIDItem->setTextAlignment(Qt::AlignCenter);
			uPPID = pe32.th32ParentProcessID;
			pPPIDItem->setData(Qt::DisplayRole, uPPID);
			ui.tableWidget->setItem(i, 2, pPPIDItem);
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(strProcessFullPath));

			i++;
			CloseHandle(hProcess);
		} while (Process32Next(hProcessSnap, &pe32));
		m_nProcessCnt = i;

		CloseHandle(hProcessSnap);
	}

	emit updateProcessNum(m_nProcessCnt);
}

bool ProcessTabWidget::getSeDebugPrivilge()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	//获取SEDEBUG特权的LUID
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//获取这个进程的关机特权
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return false;

	return true;
}

