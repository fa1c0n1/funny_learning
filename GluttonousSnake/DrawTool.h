#pragma once

class DrawTool
{
public:
	static void SetWindowSize(int cols, int lines);		//���ô��ڴ�С
	static void SetCursorPosition(const int x, const int y);	//���ù��λ��
	static void SetColor(int colorID);		//�����ı���ɫ
	static void SetBgColor();	//�����ı�����ɫ
};

