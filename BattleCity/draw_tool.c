#include "draw_tool.h"

/*****************************
函数功能：打印字符到控制台指定位置
参数1： 宽度
参数2： 高度
参数3： 打印的字符
参数4： 打印的字符的属性
返回值：无
******************************/
void WriteChar(int nWidth, int nHeight, char *szChar, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;      //是否显示光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	COORD loc;
	loc.X = nWidth * 2;
	loc.Y = nHeight;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttr);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(szChar);     // 打印时需要注意有些特殊字符是占两个字节
}

/*********************************
函数功能：设置窗口信息
参数1：窗口名称
参数2：窗口宽度
参数3：窗口高度
返回值：成功返回真
*********************************/
void SetWindowSize(char *szTitle, int nX, int nY)
{
	// 设置控制台标题
	SetConsoleTitleA(szTitle);
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