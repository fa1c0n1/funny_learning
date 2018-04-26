#include "stdafx.h"
#include "MainWindow.h"
#include "MyGlobalShortCut.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SohoSecurity w;

	//注册全局热键: Ctrl+AltShift+H ,作用是隐藏/显示主窗口
	MyGlobalShortCut *pGlobalHotKey = 
		new MyGlobalShortCut("Ctrl+Alt+Shift+H", &w); 
	QObject::connect(pGlobalHotKey, SIGNAL(activated()), &w, SLOT(activated()));

	w.show();
	return a.exec();
}
