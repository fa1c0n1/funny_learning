#pragma once

#include <windows.h>

class DrawTool
{
public:
	DrawTool();
	~DrawTool();

	static void drawPattern(int nX, int nY, int nType);
	static void drawText(int nX, int nY, char *pText, WORD wAttr);
	static void setWindowSize(char *pszTitle, int nX, int nY);

private:
	static void writeChar(int nX, int nY, char *pszChar, WORD wAttr);
};

