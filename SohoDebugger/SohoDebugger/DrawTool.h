#pragma once

#include <windows.h>
#include <QString>

/************************************************************************/
/* DrawTool�ࣺ����Ļ�����ַ������ô������ԵĹ�����
/************************************************************************/
class CDrawTool
{
public:
	CDrawTool();
	~CDrawTool();

	//���ô��ڳߴ�
	static void setWindowSize(char *pszTitle, int nX, int nY);
	//���ù��λ��
	static void setCursorPosition(const int x, const int y);
	//������ɫ
	static void setColor(int nColorID);
};

