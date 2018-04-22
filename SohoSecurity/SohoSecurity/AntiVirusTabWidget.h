#pragma once

#include <QWidget>
#include "ui_AntiVirusTabWidget.h"

class AntiVirusTabWidget : public QWidget
{
	Q_OBJECT

public:
	AntiVirusTabWidget(QWidget *parent = Q_NULLPTR);
	~AntiVirusTabWidget();

private:
	Ui::AntiVirusTabWidget ui;
};
