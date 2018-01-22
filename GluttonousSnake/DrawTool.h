#pragma once

class DrawTool
{
public:
	static void SetWindowSize(int cols, int lines);		//设置窗口大小
	static void SetCursorPosition(const int x, const int y);	//设置光标位置
	static void SetColor(int colorID);		//设置文本颜色
	static void SetBgColor();	//设置文本背景色
};

