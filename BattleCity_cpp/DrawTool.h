#pragma once

#include <windows.h>

/************************************************************************/
/* DrawTool�ࣺ����Ļ�����ַ������ô������ԵĹ�����
/************************************************************************/
class DrawTool
{
public:
	DrawTool();
	~DrawTool();

	//�������ͻ�����Ӧ��ͼ��
	static void drawPattern(int nX, int nY, int nType);
	//��ָ��λ�ô�ӡָ���ַ���
	static void drawText(int nX, int nY, char *pText, WORD wAttr);
	//���ô��ڳߴ�
	static void setWindowSize(char *pszTitle, int nX, int nY);

private:
	//��ָ��λ�ô�ӡָ���ַ���
	static void writeChar(int nX, int nY, char *pszChar, WORD wAttr);
};

