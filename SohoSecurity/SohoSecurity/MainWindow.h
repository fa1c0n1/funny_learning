#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class SohoSecurity : public QMainWindow
{
	Q_OBJECT

public:
	SohoSecurity(QWidget *parent = Q_NULLPTR);
	void initMenuEvents();

private:
	bool getPowerPrivilge();

private slots:
void onActionLockScreen();
void onActionLogOff();
void onActionHibernate();
void onActionSleep();
void onActionRestart();
void onActionShutdown();

private:
	Ui::SohoSecurityClass ui;
};
