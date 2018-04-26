#include "stdafx.h"
#include "MainWindow.h"
#include "MyGlobalShortCut.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SohoSecurity w;

	//ע��ȫ���ȼ�: Ctrl+AltShift+H ,����������/��ʾ������
	MyGlobalShortCut *pGlobalHotKey = 
		new MyGlobalShortCut("Ctrl+Alt+Shift+H", &w); 
	QObject::connect(pGlobalHotKey, SIGNAL(activated()), &w, SLOT(activated()));

	w.show();
	return a.exec();
}
