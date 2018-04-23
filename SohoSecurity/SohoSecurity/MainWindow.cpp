#include "stdafx.h"
#include "MainWindow.h"
#include "ProcessTabWidget.h"
#include "CpuMemInfoThread.h"
#include <PowrProf.h>

#pragma comment(lib, "PowrProf.lib")

SohoSecurity::SohoSecurity(QWidget *parent)
	: QMainWindow(parent), m_pCmInfoThread(nullptr), m_pStatusBar(nullptr),
	m_pLabelCpuUsage(nullptr), m_pLabelMemUsage(nullptr)
{
	ui.setupUi(this);

	m_pStatusBar = statusBar();
	m_pLabelCpuUsage = new QLabel(tr("CPU Usage:     "), this);
	m_pLabelMemUsage = new QLabel(tr("Physical Memory:     "), this);
	m_pStatusBar->addWidget(m_pLabelCpuUsage);
	m_pStatusBar->addWidget(m_pLabelMemUsage);

	////状态栏
	//QStatusBar *sBar = statusBar();
	//QLabel *label = new QLabel(this);
	//label->setText("CPU Usage: 4%");
	//sBar->addWidget(label);
	////addWidget 从左往右添加
	//sBar->addWidget(new QLabel("Physical Memory: 67%", this));

	//// addPermanentWidget 从右往左添加
	//sBar->addPermanentWidget(new QLabel("3", this));

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

	//当按窗口右上角x时，窗口触发destroyed
	//connect(this, &SohoSecurity::destroyed, this, &SohoSecurity::onStopThread);
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

void SohoSecurity::onUpdateCpuUsage(int nCpuUsage)
{
	m_pLabelCpuUsage->setText(QString::asprintf("CPU Usage: %d%%    ", nCpuUsage));
}

void SohoSecurity::onUpdateMemUsage(int dwMemUsage)
{
	m_pLabelMemUsage->setText(QString::asprintf("Physical Memory: %d%%    ", dwMemUsage));
}

void SohoSecurity::onStopThread()
{
	//停止线程
	//m_pCmInfoThread->quit();
	//等待线程处理完手头动作
	//m_pCmInfoThread->wait();
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
