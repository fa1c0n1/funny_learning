#include <windows.h>
#include <cstring>
#include <cstdio>
#include "DrawTool.h"


void DrawTool::SetWindowSize(int cols, int lines)//设置窗口大小
{
	system("title Gluttonous Snake");//设置窗口标题
	char cmd[64];
	sprintf_s(cmd, _countof(cmd), "mode con cols=%d lines=%d", cols * 2, lines);//一个图形■占两个字符，故宽度乘以2
	system(cmd);	//system(mode con cols=88 lines=88)设置窗口宽度和高度
}

void DrawTool::SetCursorPosition(const int x, const int y)//设置光标位置
{
	COORD position;
	position.X = x * 2;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void DrawTool::SetColor(int colorID)//设置文本颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

void DrawTool::SetBgColor()//设置文本背景色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_BLUE |
		BACKGROUND_BLUE |
		BACKGROUND_GREEN |
		BACKGROUND_RED);
}
