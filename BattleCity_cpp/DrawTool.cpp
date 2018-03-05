#include "DrawTool.h"
#include "Data.h"
#include <cstdio>

DrawTool::DrawTool()
{
}


DrawTool::~DrawTool()
{
}

void DrawTool::drawPattern(int nX, int nY, int nType)
{
	switch (nType)
	{
	case SIGN_EMPTY:
		writeChar(nX, nY, "  ", 0);
		break;
	case SIGN_WALL0:
		writeChar(nX, nY, "■", FG_HIGHWHITE);
		break;
	case SIGN_WALL1:
		writeChar(nX, nY, "■", FG_YELLOW);
		break;
	case SIGN_TANK_PA:
		writeChar(nX, nY, "■", FG_LIGHTTURQUOISE);
		break;
	case SIGN_TANK_PB:
		writeChar(nX, nY, "■", FG_LIGHTGREEN);
		break;
	case SIGN_TANK_E0:
		writeChar(nX, nY, "■", FG_LIGHTRED);
		break;
	case SIGN_TANK_E1:
		writeChar(nX, nY, "■", FG_LIGHTPURPLE);
		break;
	case SIGN_GRASS:
		writeChar(nX, nY, "▓", FG_GREEN);
		break;
	case SIGN_RIVER:
		writeChar(nX, nY, "▓", FG_LIGHTBLUE);
		break;
	case SIGN_BULLET0:
		writeChar(nX, nY, "●", FG_LIGHTYELLOW);
		break;
	case SIGN_BULLET1:
		writeChar(nX, nY, "●", FG_GRAY);
		break;
	case SIGN_PILLBOX:
		writeChar(nX, nY, "★", FG_GRAY);
		break;
	case SIGN_PILLBOX_HIT:
		writeChar(nX, nY, "★", FG_RED);
		break;
	}
}

void DrawTool::drawText(int nX, int nY, char *pText, WORD wAttr)
{
	writeChar(nX, nY, pText, wAttr);
}

void DrawTool::writeChar(int nX, int nY, char *pszChar, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;      //是否显示光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	COORD loc;
	loc.X = nX * 2;
	loc.Y = nY;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttr);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar);     // 打印时需要注意有些特殊字符是占两个字节
}

void DrawTool::setWindowSize(char *pszTitle, int nX, int nY)
{
	// 设置控制台标题
	SetConsoleTitleA(pszTitle);
	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// 获取最大控制台窗口大小
	COORD pos = GetLargestConsoleWindowSize(hStdOut);

	// 设置控制台缓冲区大小
	COORD BufferSize = { pos.X + 1, pos.Y + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, BufferSize))
	{
		// 设置控制台窗口缓冲区失败
		printf("buffer err (%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return;
	}

	// 设置控制台窗口大小
	SMALL_RECT srctWindow = { 0, 0, nX, nY };
	if (!SetConsoleWindowInfo(hStdOut, TRUE, &srctWindow))
	{
		printf("size err %d\n", GetLastError());
		return;
	}

	// 设置控制台缓冲区大小
	COORD Buffer = { nX + 1, nY + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, Buffer))
	{
		// 设置控制台窗口缓冲区失败
		printf("buffer err (%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return;
	}
}
