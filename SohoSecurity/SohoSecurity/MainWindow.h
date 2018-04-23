#pragma once

#include "ui_MainWindow.h"
#include "CpuMemInfoThread.h"
#include <QtWidgets/QMainWindow>

class SohoSecurity : public QMainWindow
{
	Q_OBJECT

public:
	SohoSecurity(QWidget *parent = Q_NULLPTR);
	void initSlotConnect();

private:
	bool getPowerPrivilge();

private slots:
void onActionLockScreen();
void onActionLogOff();
void onActionHibernate();
void onActionSleep();
void onActionRestart();
void onActionShutdown();
void onUpdateCpuUsage(int nCpuUsage);
void onUpdateMemUsage(int dwMemUsage);
void onStopThread();

private:
	Ui::SohoSecurityClass ui;
	CpuMemInfoThread *m_pCmInfoThread;
	QStatusBar *m_pStatusBar;
	QLabel *m_pLabelCpuUsage;
	QLabel *m_pLabelMemUsage;
};
