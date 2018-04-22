#pragma once

#include <QWidget>
#include "ui_WindowTabWidget.h"

class WindowTabWidget : public QWidget
{
	Q_OBJECT

public:
	WindowTabWidget(QWidget *parent = Q_NULLPTR);
	~WindowTabWidget();

private:
	Ui::WindowTabWidget ui;
};
