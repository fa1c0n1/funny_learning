#pragma once

#include <QWidget>
#include "ui_StartItemTabWidget.h"

class StartItemTabWidget : public QWidget
{
	Q_OBJECT

public:
	StartItemTabWidget(QWidget *parent = Q_NULLPTR);
	~StartItemTabWidget();

protected:
	void timerEvent(QTimerEvent *tEvent);

private:
	void listStartItem();

private:
	Ui::StartItemTabWidget ui;
	int m_nTimerId;
};
