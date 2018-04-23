#pragma once

#include <QWidget>
#include "ui_WindowTabWidget.h"

class WindowTabWidget : public QWidget
{
	Q_OBJECT

public:
	WindowTabWidget(QWidget *parent = Q_NULLPTR);
	~WindowTabWidget();

private slots:
void onTableWidgetCustomContextMenuRequest(QPoint pos);
void onPopMenuRefresh();

private:
	void listWindow();
	static BOOL CALLBACK enumWndProc(HWND hwnd, LPARAM lParam);

private:
	Ui::WindowTabWidget ui;
	static int m_nWndCnt;
};
