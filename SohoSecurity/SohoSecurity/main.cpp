#include "stdafx.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SohoSecurity w;
	w.show();
	return a.exec();
}
