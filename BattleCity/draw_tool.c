#include "draw_tool.h"

/*****************************
�������ܣ���ӡ�ַ�������ָ̨��λ��
����1�� ���
����2�� �߶�
����3�� ��ӡ���ַ�
����4�� ��ӡ���ַ�������
����ֵ����
******************************/
void WriteChar(int nWidth, int nHeight, char *szChar, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;      //�Ƿ���ʾ���
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	COORD loc;
	loc.X = nWidth * 2;
	loc.Y = nHeight;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttr);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(szChar);     // ��ӡʱ��Ҫע����Щ�����ַ���ռ�����ֽ�
}

/*********************************
�������ܣ����ô�����Ϣ
����1����������
����2�����ڿ��
����3�����ڸ߶�
����ֵ���ɹ�������
*********************************/
void SetWindowSize(char *szTitle, int nX, int nY)
{
	// ���ÿ���̨����
	SetConsoleTitleA(szTitle);
	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// ��ȡ������̨���ڴ�С
	COORD pos = GetLargestConsoleWindowSize(hStdOut);

	// ���ÿ���̨��������С
	COORD BufferSize = { pos.X + 1, pos.Y + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, BufferSize))
	{
		// ���ÿ���̨���ڻ�����ʧ��
		printf("buffer err (%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return;
	}

	// ���ÿ���̨���ڴ�С
	SMALL_RECT srctWindow = { 0, 0, nX, nY };
	if (!SetConsoleWindowInfo(hStdOut, TRUE, &srctWindow))
	{
		printf("size err %d\n", GetLastError());
		return;
	}

	// ���ÿ���̨��������С
	COORD Buffer = { nX + 1, nY + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, Buffer))
	{
		// ���ÿ���̨���ڻ�����ʧ��
		printf("buffer err (%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return;
	}
}