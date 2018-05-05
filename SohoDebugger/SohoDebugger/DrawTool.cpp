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

void CDrawTool::setWindowSize(char *pszTitle, int nX, int nY)
{
	system(pszTitle);//���ô��ڱ���
	char cmd[64];
	sprintf_s(cmd, _countof(cmd), "mode con cols=%d lines=%d", 100, 40);//һ��ͼ�Ρ�ռ�����ַ����ʿ�ȳ���2
	system(cmd);	//system(mode con cols=88 lines=88)���ô��ڿ�Ⱥ͸߶�
}

void CDrawTool::setCursorPosition(const int x, const int y)
{
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}