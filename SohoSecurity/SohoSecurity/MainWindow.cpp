#include "stdafx.h"
#include "MainWindow.h"
#include "ProcessTabWidget.h"
#include "CpuMemInfoThread.h"
#include <Psapi.h>
#include <PowrProf.h>

#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "Psapi.lib")

SohoSecurity::SohoSecurity(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowIcon(QIcon(QStringLiteral(":SohoSecurity/Security_Center.ico")));
	this->setFixedSize(this->width(), this->height());

	m_pStatusBar = statusBar();
	m_pLabelProcCnt = new QLabel(tr("进程数: "), this);
	m_pLabelCpuUsage = new QLabel(tr("CPU 使用率: "), this);
	m_pLabelMemUsage = new QLabel(tr("物理内存: "), this);
	m_pLabelProcCnt->setFixedWidth(100);
	m_pLabelCpuUsage->setFixedWidth(120);
	m_pLabelMemUsage->setFixedWidth(180);
	m_pStatusBar->addWidget(m_pLabelProcCnt);
	m_pStatusBar->addWidget(m_pLabelCpuUsage);
	m_pStatusBar->addWidget(m_pLabelMemUsage);

	m_pLabelMemOptimizeInfo = new QLabel(tr(""));
	m_pLabelMemOptimizeInfo->setFixedWidth(100);
	m_pStatusBar->addPermanentWidget(m_pLabelMemOptimizeInfo);
	m_pPsBtnOptimizeMem = new QPushButton(tr("内存优化"), this);
	m_pStatusBar->addPermanentWidget(m_pPsBtnOptimizeMem);

	m_pProcTabWidget = new ProcessTabWidget(this);
	m_pWndTabWidget = new WindowTabWidget(this);
	m_pServTabWidget = new ServiceTabWidget(this);
	m_pPETabWidget = new PETabWidget(this);
	m_pCleanTabWidget = new CleanTrashTabWidget(this);
	m_pSoftUnsTabWidget = new SoftUninstallTabWidget(this);
	m_pStartItemTabWidget = new StartItemTabWidget(this);
	m_pAVTabWidget = new AntiVirusTabWidget(this);
	ui.tabWidget->addTab(m_pProcTabWidget, tr("进程"));
	ui.tabWidget->addTab(m_pWndTabWidget, tr("窗口"));
	ui.tabWidget->addTab(m_pServTabWidget, tr("服务"));
	ui.tabWidget->addTab(m_pPETabWidget, tr("PE文件解析"));
	ui.tabWidget->addTab(m_pCleanTabWidget, tr("垃圾清理"));
	ui.tabWidget->addTab(m_pSoftUnsTabWidget, tr("软件管理"));
	ui.tabWidget->addTab(m_pStartItemTabWidget, tr("启动项"));
	ui.tabWidget->addTab(m_pAVTabWidget, tr("病毒查杀"));

	m_pCmInfoThread = new CpuMemInfoThread(this);
	m_pCmInfoThread->start();

	initSlotConnect();
}

void SohoSecurity::initSlotConnect()
{
	connect(ui.actLockScreen, &QAction::triggered, this, &SohoSecurity::onActionLockScreen);
	connect(ui.actLogoff, &QAction::triggered, this, &SohoSecurity::onActionLogOff);
	connect(ui.actHibernate, &QAction::triggered, this, &SohoSecurity::onActionHibernate);
	connect(ui.actSleep, &QAction::triggered, this, &SohoSecurity::onActionSleep);
	connect(ui.actRestart, &QAction::triggered, this, &SohoSecurity::onActionRestart);
	connect(ui.actShutdown, &QAction::triggered, this, &SohoSecurity::onActionShutdown);

	connect(m_pCmInfoThread, &CpuMemInfoThread::updateCpuUsage, this, &SohoSecurity::onUpdateCpuUsage);
	connect(m_pCmInfoThread, &CpuMemInfoThread::updateMemUsage, this, &SohoSecurity::onUpdateMemUsage);

	connect(m_pPsBtnOptimizeMem, &QPushButton::clicked, this, &SohoSecurity::onUpdateMemOptimizeInfo);

	//当按窗口右上角x时，窗口触发destroyed
	connect(this, &SohoSecurity::destroyed, this, &SohoSecurity::onStopThread);
}

void SohoSecurity::activated()
{
	if (isHidden()) {
		show();
	}
	else {
		hide();
	}
}

void SohoSecurity::onActionLockScreen()
{
	LockWorkStation();
}

void SohoSecurity::onActionLogOff()
{
	if (QMessageBox::question(this, tr("询问"), tr("确定要注销吗?")) == QMessageBox::Yes)
		ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
}

void SohoSecurity::onActionHibernate()
{
	if (QMessageBox::question(this, tr("询问"), tr("确定要休眠吗?")) == QMessageBox::Yes)
		SetSuspendState(TRUE, FALSE, FALSE);
}

void SohoSecurity::onActionSleep()
{
	if (QMessageBox::question(this, tr("询问"), tr("确定要睡眠吗?")) == QMessageBox::Yes)
		SetSuspendState(FALSE, FALSE, FALSE);
}

void SohoSecurity::onActionRestart()
{
	if (QMessageBox::question(this, tr("询问"), tr("确定要重启吗?")) == QMessageBox::Yes) {
		if (getPowerPrivilge())
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
	}
}

void SohoSecurity::onActionShutdown()
{
	if (QMessageBox::question(this, tr("询问"), tr("确定要关机吗?")) == QMessageBox::Yes) {
		if (getPowerPrivilge())
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
	}
}

void SohoSecurity::onUpdateProcessNum(int nProcNum)
{
	m_pLabelProcCnt->setText(QString::asprintf("进程数: %d", nProcNum));
}

void SohoSecurity::onUpdateCpuUsage(int nCpuUsage)
{
	m_pLabelCpuUsage->setText(QString::asprintf("CPU 使用率: %d%%", nCpuUsage));
}

void SohoSecurity::onUpdateMemUsage(int dwMemUsage)
{
	m_pLabelMemUsage->setText(QString::asprintf("物理内存: %d%%", dwMemUsage));
}

void SohoSecurity::onStopThread()
{
	//停止线程
	m_pCmInfoThread->setQuitThreadFlag(true);
	m_pCmInfoThread->quit();
	//等待线程处理完手头动作
	m_pCmInfoThread->wait();
}

void SohoSecurity::onUpdateMemOptimizeInfo()
{
	m_pLabelMemOptimizeInfo->setText(tr("正在优化内存..."));
	
	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD dwBufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;

	EnumProcesses(dwPIDList, dwBufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
		CloseHandle(hProcess);
	}

	m_pLabelMemOptimizeInfo->setText(tr("内存优化完毕"));
}

bool SohoSecurity::getPowerPrivilge()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	//获取关机特权的LUID
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//获取这个进程的关机特权
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return false;

	return true;
}
