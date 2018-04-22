#pragma once

#include <QWidget>
#include "ui_ServiceTabWidget.h"

class ServiceTabWidget : public QWidget
{
	Q_OBJECT

public:
	ServiceTabWidget(QWidget *parent = Q_NULLPTR);
	~ServiceTabWidget();

private:
	Ui::ServiceTabWidget ui;
};
