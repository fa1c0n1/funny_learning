#include "stdafx.h"
#include "MainWindow.h"
#include "MyGlobalShortCut.h"
#include <QtWidgets/QApplication>

HANDLE g_hMutex = 0;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SohoSecurity w;

	//ʹ�û�����ʵ�ֳ���ķ��࿪
	if (OpenMutex(SYNCHRONIZE, NULL, TEXT("SohoSecurity_fa1c0n1_1107"))) {
		return 0;
	}
	g_hMutex = CreateMutex(NULL, NULL, TEXT("SohoSecurity_fa1c0n1_1107"));

	//ע��ȫ���ȼ�: Ctrl+AltShift+H ,����������/��ʾ������
	MyGlobalShortCut *pGlobalHotKey = 
		new MyGlobalShortCut("Ctrl+Alt+Shift+H", &w); 
	QObject::connect(pGlobalHotKey, SIGNAL(activated()), &w, SLOT(activated()));

	w.show();
	return a.exec();
}
