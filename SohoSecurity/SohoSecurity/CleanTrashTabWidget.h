#pragma once

#include <QWidget>
#include "ui_CleanTrashTabWidget.h"

class CleanTrashTabWidget : public QWidget
{
	Q_OBJECT

public:
	CleanTrashTabWidget(QWidget *parent = Q_NULLPTR);
	~CleanTrashTabWidget();

private:
	Ui::CleanTrashTabWidget ui;
};
