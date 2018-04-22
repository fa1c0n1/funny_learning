#include "stdafx.h"
#include "ThreadDlg.h"
#include "Data.h"
#include <TlHelp32.h>

ThreadDlg::ThreadDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ThreadDlg::ThreadDlg(LPVOID lpData, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	PPROCESS2THREADINFO pP2tInfo = (PPROCESS2THREADINFO)lpData;
	m_dwOwnerPID = pP2tInfo->dwPID;
	m_strOwnerPName = pP2tInfo->strPName;

	QStringList headers;
	headers << "�߳�ID" << "���ȼ�" << "״̬";
	ui.tableWidgetThread->setColumnCount(3);
	ui.tableWidgetThread->setHorizontalHeaderLabels(headers);
	ui.tableWidgetThread->setColumnWidth(0, ui.tableWidgetThread->width() / 3);
	ui.tableWidgetThread->setColumnWidth(1, ui.tableWidgetThread->width() / 3);
	ui.tableWidgetThread->setColumnWidth(2, ui.tableWidgetThread->width() / 3);

	ListProcessThread(m_dwOwnerPID);
	setWindowTitle(QString("[%1]").arg(m_strOwnerPName) + QString::asprintf("�����߳�(%d)", m_nThreadNum));

	ui.tableWidgetThread->setContextMenuPolicy(Qt::CustomContextMenu);
}

ThreadDlg::~ThreadDlg()
{
}

void ThreadDlg::onTableWidgetCustomContextMenuRequest(QPoint pos)
{
	QTableWidgetItem *pCurItem = ui.tableWidgetThread->itemAt(pos);
	if (pCurItem == nullptr)
		return;

	QMenu *pPopMenu = new QMenu(ui.tableWidgetThread);
	QAction *pActRefresh = pPopMenu->addAction("ˢ��");
	QAction *pActSuspendThread = pPopMenu->addAction("�����߳�");
	QAction *pActResumeThread = pPopMenu->addAction("�ָ��߳�");
	QAction *pActEndThread = pPopMenu->addAction("�����߳�");

	connect(pActRefresh, &QAction::triggered, this, &ThreadDlg::onPopMenuRefresh);
	connect(pActSuspendThread, &QAction::triggered, this, &ThreadDlg::onPopMenuSuspendThread);
	connect(pActResumeThread, &QAction::triggered, this, &ThreadDlg::onPopMenuResumeThread);
	connect(pActEndThread, &QAction::triggered, this, &ThreadDlg::onPopMenuEndThread);

	pPopMenu->exec(QCursor::pos());

	delete pActRefresh; pActRefresh = nullptr;
	delete pActSuspendThread; pActSuspendThread = nullptr;
	delete pActResumeThread; pActResumeThread = nullptr;
	delete pActEndThread; pActEndThread = nullptr;
	delete pPopMenu; pPopMenu = nullptr;
}

void ThreadDlg::onPopMenuRefresh()
{
	ui.tableWidgetThread->clearContents();
	ui.tableWidgetThread->setSortingEnabled(false); //->��������ˢ�����ݻ���ɿؼ������ݺܶ඼Ϊ�հ׵�bug
	ListProcessThread(m_dwOwnerPID);
	ui.tableWidgetThread->setSortingEnabled(true);
}

void ThreadDlg::onPopMenuSuspendThread()
{

}

void ThreadDlg::onPopMenuResumeThread()
{

}

void ThreadDlg::onPopMenuEndThread()
{

}

void ThreadDlg::ListProcessThread(DWORD dwOwnerPid)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	HANDLE hThread;
	THREADENTRY32 te32 = {};
	UINT uThreadID;
	QString strThreadState;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32))
	{
		//TODO:
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	int i = 0;
	do {
		if (te32.th32OwnerProcessID == dwOwnerPid) {
			ui.tableWidgetThread->setRowCount(i + 1);
			QTableWidgetItem *pThreadIDItem = new QTableWidgetItem;
			uThreadID = te32.th32ThreadID;
			pThreadIDItem->setData(Qt::DisplayRole, uThreadID);
			ui.tableWidgetThread->setItem(i, 0, pThreadIDItem);
			
			QTableWidgetItem *pThreadPriItem = new QTableWidgetItem;
			pThreadPriItem->setData(Qt::DisplayRole, te32.tpBasePri);
			ui.tableWidgetThread->setItem(i, 1, pThreadPriItem);

			hThread = OpenThread(THREAD_SUSPEND_RESUME, NULL, te32.th32ThreadID);
			if (hThread) {
				//ͨ��SuspendThread�����ķ���ֵ(�������)���ж��̵߳�״̬
				int nSuspendCnt = SuspendThread(hThread);
				if (nSuspendCnt == 0) {
					ResumeThread(hThread);
					strThreadState = "����";
				}
				else if (nSuspendCnt > 0) {
					ResumeThread(hThread);
					strThreadState = "����";
				}
				else {
					strThreadState = "����";
				}
			}
			else {
				strThreadState = "����";
			}
			ui.tableWidgetThread->setItem(i, 2, new QTableWidgetItem(strThreadState));
			i++;
		}
	} while (Thread32Next(hThreadSnap, &te32));
	m_nThreadNum = i;

	CloseHandle(hThreadSnap);
}
