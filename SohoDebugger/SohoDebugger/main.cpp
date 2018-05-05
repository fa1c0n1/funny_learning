#include "stdafx.h"
#include "Comm.h"
#include "DebuggerMain.h"
#include "DrawTool.h"
#include <QtCore/QCoreApplication>



int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CDrawTool::setWindowSize("title SohoDebugger", 100, 40);

	CDebuggerMain debuggerMain;
	debuggerMain.launchDebugger();

	return a.exec();
}
