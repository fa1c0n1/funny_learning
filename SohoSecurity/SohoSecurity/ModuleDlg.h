#pragma once

#include <QDialog>
#include "ui_ModuleDlg.h"

class ModuleDlg : public QDialog
{
	Q_OBJECT

public:
	ModuleDlg(QWidget *parent = Q_NULLPTR);
	ModuleDlg(LPVOID lpData, QWidget *parent = Q_NULLPTR);
	~ModuleDlg();

private:
	void ListProcessThread(DWORD dwOwnerPid);

private:
	Ui::ModuleDlg ui;
	DWORD m_dwOwnerPID;
	int m_nModuleNum;
	QString m_strOwnerPName;
};
