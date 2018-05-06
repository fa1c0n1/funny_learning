#include "DrawTool.h"

CDrawTool::CDrawTool()
{
}

CDrawTool::~CDrawTool()
{
}

void CDrawTool::setColor(int nColorID)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), nColorID);
}

void CDrawTool::setWindowTitle(const char *pszTitle)
{
	char cmd[128] = {};
	sprintf_s(cmd, _countof(cmd), "title %s", pszTitle);//一个图形■占两个字符，故宽度乘以2
	system(cmd);	//system(mode con cols=88 lines=88)设置窗口宽度和高度
}

void CDrawTool::setWindowSize(int nX, int nY)
{
	char cmd[64] = {};
	sprintf_s(cmd, _countof(cmd), "mode con cols=%d lines=%d", 100, 40);//一个图形■占两个字符，故宽度乘以2
	system(cmd);	//system(mode con cols=88 lines=88)设置窗口宽度和高度
}

void CDrawTool::setCursorPosition(const int x, const int y)
{
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}