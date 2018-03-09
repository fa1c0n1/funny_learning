#pragma once

#include <windows.h>

/************************************************************************/
/* DrawTool类：在屏幕画出字符、设置窗口属性的工具类
/************************************************************************/
class DrawTool
{
public:
	DrawTool();
	~DrawTool();

	//根据类型画出对应的图案
	static void drawPattern(int nX, int nY, int nType);
	//在指定位置打印指定字符串
	static void drawText(int nX, int nY, char *pText, WORD wAttr);
	//设置窗口尺寸
	static void setWindowSize(char *pszTitle, int nX, int nY);

private:
	//在指定位置打印指定字符串
	static void writeChar(int nX, int nY, char *pszChar, WORD wAttr);
};

