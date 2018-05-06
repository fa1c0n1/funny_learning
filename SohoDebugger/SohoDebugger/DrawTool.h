#pragma once

#include <windows.h>
#include <QString>

/************************************************************************/
/* DrawTool类：在屏幕画出字符、设置窗口属性的工具类
/************************************************************************/
class CDrawTool
{
public:
	CDrawTool();
	~CDrawTool();

	static void setWindowTitle(const char *pszTitle);
	//设置窗口尺寸
	static void setWindowSize(int nX, int nY);
	//设置光标位置
	static void setCursorPosition(const int x, const int y);
	//设置颜色
	static void setColor(int nColorID);
};

