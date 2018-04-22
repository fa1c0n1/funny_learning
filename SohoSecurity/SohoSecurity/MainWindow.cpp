#include "stdafx.h"
#include "MainWindow.h"
#include "ProcessTabWidget.h"
#include <PowrProf.h>

#pragma comment(lib, "PowrProf.lib")

SohoSecurity::SohoSecurity(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initMenuEvents();
}

void SohoSecurity::initMenuEvents()
{
	connect(ui.actLockScreen, &QAction::triggered, this, &SohoSecurity::onActionLockScreen);
	connect(ui.actLogoff, &QAction::triggered, this, &SohoSecurity::onActionLogOff);
	connect(ui.actHibernate, &QAction::triggered, this, &SohoSecurity::onActionHibernate);
	connect(ui.actSleep, &QAction::triggered, this, &SohoSecurity::onActionSleep);
	connect(ui.actRestart, &QAction::triggered, this, &SohoSecurity::onActionRestart);
	connect(ui.actShutdown, &QAction::triggered, this, &SohoSecurity::onActionShutdown);
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
