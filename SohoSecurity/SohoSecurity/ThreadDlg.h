#pragma once

#include <QDialog>
#include "ui_ThreadDlg.h"

class ThreadDlg : public QDialog
{
	Q_OBJECT

public:
	ThreadDlg(QWidget *parent = Q_NULLPTR);
	ThreadDlg(LPVOID lpData, QWidget *parent = Q_NULLPTR);
	~ThreadDlg();

	void ListProcessThread(DWORD dwOwnerPid);


private slots:
void onTableWidgetCustomContextMenuRequest(QPoint pos);
void onPopMenuRefresh();
void onPopMenuSuspendThread();
void onPopMenuResumeThread();
void onPopMenuEndThread();

private:
	Ui::ThreadDlg ui;
	DWORD m_dwOwnerPID;
	int m_nThreadNum;
	QString m_strOwnerPName;
};
