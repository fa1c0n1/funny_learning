#pragma once

#include "ui_MainWindow.h"
#include "CpuMemInfoThread.h"
#include "ProcessTabWidget.h"
#include "WindowTabWidget.h"
#include "ServiceTabWidget.h"
#include "PETabWidget.h"
#include "CleanTrashTabWidget.h"
#include "SoftUninstallTabWidget.h"
#include "StartItemTabWidget.h"
#include "AntiVirusTabWidget.h"
#include <QtWidgets/QMainWindow>

class SohoSecurity : public QMainWindow
{
	Q_OBJECT

public:
	SohoSecurity(QWidget *parent = Q_NULLPTR);
	void initSlotConnect();

private:
	bool getPowerPrivilge();

public slots:
void activated();
void onActionLockScreen();
void onActionLogOff();
void onActionHibernate();
void onActionSleep();
void onActionRestart();
void onActionShutdown();
void onUpdateProcessNum(int nProcNum);
void onUpdateCpuUsage(int nCpuUsage);
void onUpdateMemUsage(int dwMemUsage);
void onStopThread();
void onUpdateMemOptimizeInfo();

private:
	Ui::SohoSecurityClass ui;
	ProcessTabWidget *m_pProcTabWidget;
	WindowTabWidget *m_pWndTabWidget;
	ServiceTabWidget *m_pServTabWidget;
	PETabWidget *m_pPETabWidget;
	CleanTrashTabWidget *m_pCleanTabWidget;
	SoftUninstallTabWidget *m_pSoftUnsTabWidget;
	StartItemTabWidget *m_pStartItemTabWidget;
	AntiVirusTabWidget *m_pAVTabWidget;
	CpuMemInfoThread *m_pCmInfoThread;
	QStatusBar *m_pStatusBar;
	QLabel *m_pLabelProcCnt;
	QLabel *m_pLabelCpuUsage;
	QLabel *m_pLabelMemUsage;
	QLabel *m_pLabelMemOptimizeInfo;
	QPushButton *m_pPsBtnOptimizeMem;
};
