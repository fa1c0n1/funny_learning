#include "stdafx.h"
#include "ModuleDlg.h"
#include "Data.h"
#include <TlHelp32.h>

ModuleDlg::ModuleDlg(QWidget *parent)
	: QDialog(parent), m_dwOwnerPID(0), m_nModuleNum(0)
{
	ui.setupUi(this);
}

ModuleDlg::ModuleDlg(LPVOID lpData, QWidget *parent)
	: QDialog(parent), m_nModuleNum(0)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	PPROCESS2MODULEINFO pP2mInfo = (PPROCESS2MODULEINFO)lpData;
	m_dwOwnerPID = pP2mInfo->dwPID;
	m_strOwnerPName = pP2mInfo->strPName;

	QStringList headers;
	headers << "模块路径" << "基地址" << "大小";
	ui.tableWidgetModule->setColumnCount(3);
	ui.tableWidgetModule->setHorizontalHeaderLabels(headers);
	ui.tableWidgetModule->setColumnWidth(0, ui.tableWidgetModule->width() / 3);
	ui.tableWidgetModule->setColumnWidth(1, ui.tableWidgetModule->width() / 3);
	ui.tableWidgetModule->setColumnWidth(2, ui.tableWidgetModule->width() / 3);

	ListProcessThread(m_dwOwnerPID);
	setWindowTitle(QString("[%1]").arg(m_strOwnerPName) + QString::asprintf("进程模块(%d)", m_nModuleNum));
}

ModuleDlg::~ModuleDlg()
{
}

//遍历模块
void ModuleDlg::ListProcessThread(DWORD dwOwnerPid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = {};
	QString strModuleBaseAddr;
	QString strModuleSize;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwOwnerPid);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	me32.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hModuleSnap, &me32)) {
		//TODO:
		CloseHandle(hModuleSnap);
		return;
	}
	else {
		int i = 0;
		do
		{
			ui.tableWidgetModule->setRowCount(i + 1);
			ui.tableWidgetModule->setItem(i, 0, new QTableWidgetItem(QString::fromWCharArray(me32.szExePath)));
			QTableWidgetItem *pModBaseAddrItem = new QTableWidgetItem(QString::asprintf("0x%016X", (LONG64)me32.modBaseAddr));
			pModBaseAddrItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidgetModule->setItem(i, 1, pModBaseAddrItem);
			QTableWidgetItem *pModBaseSizeItem = new QTableWidgetItem(QString::asprintf("0x%016X", me32.modBaseSize));
			pModBaseSizeItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidgetModule->setItem(i, 2, pModBaseSizeItem);
			i++;
		} while (Module32Next(hModuleSnap, &me32));
		m_nModuleNum = i;

		CloseHandle(hModuleSnap);
	}
}
