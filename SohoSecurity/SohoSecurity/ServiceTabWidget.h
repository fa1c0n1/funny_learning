#pragma once

#include <QWidget>
#include "ui_ServiceTabWidget.h"

class ServiceTabWidget : public QWidget
{
	Q_OBJECT

public:
	ServiceTabWidget(QWidget *parent = Q_NULLPTR);
	~ServiceTabWidget();

public slots :
void onTableWidgetCustomContextMenuRequest(QPoint pos);
void onPopMenuRefresh();
void onPopMenuStartService();
void onPopMenuStopService();

private:
	void listService();
	bool startService();
	bool stopService();

private:
	Ui::ServiceTabWidget ui;
	QMenu *m_pPopMenu;
	QAction *m_pActStartService;
	QAction *m_pActStopService;
};
