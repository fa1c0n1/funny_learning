#include <windows.h>
#include <cstring>
#include <cstdio>
#include "DrawTool.h"


void DrawTool::SetWindowSize(int cols, int lines)//���ô��ڴ�С
{
	system("title Gluttonous Snake");//���ô��ڱ���
	char cmd[64];
	sprintf_s(cmd, _countof(cmd), "mode con cols=%d lines=%d", cols * 2, lines);//һ��ͼ�Ρ�ռ�����ַ����ʿ�ȳ���2
	system(cmd);	//system(mode con cols=88 lines=88)���ô��ڿ�Ⱥ͸߶�
}

void DrawTool::SetCursorPosition(const int x, const int y)//���ù��λ��
{
	COORD position;
	position.X = x * 2;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void DrawTool::SetColor(int colorID)//�����ı���ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

void DrawTool::SetBgColor()//�����ı�����ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_BLUE |
		BACKGROUND_BLUE |
		BACKGROUND_GREEN |
		BACKGROUND_RED);
}
