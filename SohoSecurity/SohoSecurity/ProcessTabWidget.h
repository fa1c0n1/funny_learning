#pragma once

#include <QWidget>
#include "ui_ProcessTabWidget.h"

class ProcessTabWidget : public QWidget
{
	Q_OBJECT

public:
	ProcessTabWidget(QWidget *parent = Q_NULLPTR);
	~ProcessTabWidget();

public slots :
void onTableWidgetCustomContextMenuRequest(QPoint pos);
void onPopMenuRefresh();
void onPopMenuTraverseModule();
void onPopMenuTraverseThread();
void onPopMenuEndProcess();

signals:
void updateProcessNum(int nProcNum);

private:
	void listProcess();
	bool endProcess(DWORD dwPID);
	bool getSeDebugPrivilge();

private:
	Ui::ProcessTabWidget ui;
	int m_nProcessCnt;
};
