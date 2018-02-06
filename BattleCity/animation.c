#include "draw_tool.h"
#include "animation.h"
#include "data.h"

void prnStartTank(void)
{
	int dx1 = 0, dx2 = 0;
	int nBullets[2] = { 1, 1 };   //两发子弹

	while (dx1 < 50) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (g_TankStatus[3][i][j] == 1) {
					if (j + dx1 >= 50)
						WriteChar(j + dx1, i + 10, "  ", 0);
					else
						WriteChar(j + dx1, i + 10, "■", FG_LIGHTTURQUOISE);
				}
			}
		}

		for (int i = 0; i < 2; i++) {
			if (nBullets[i] == 1) {
				if (i + 3 + dx2 >= 50) {
					nBullets[i] = 0;
					WriteChar(i + 3 + dx2, 1 + 10, "  ", 0);
				}
				else {
					WriteChar(i + 3 + dx2, 1 + 10, "●", FG_LIGHTYELLOW);
				}
			}
		}

		Sleep(30);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (g_TankStatus[3][i][j] == 1)
					WriteChar(j + dx1, i + 10, "  ", 0);
			}
		}

		for (int i = 0; i < 2; i++) {
			if (nBullets[i] == 1)
				WriteChar(i + 3 + dx2, 1 + 10, "  ", 0);
		}

		dx1++;
		dx2 += 3;
	}
}

void prnStartText1(void)
{
	while (g_text1[_countof(g_text1) - 1].nX < 40) {
		for (int i = 0; i < _countof(g_text1); i++) {
			if (g_text1[i].nX >= 0)
				WriteChar(g_text1[i].nX, g_text1[i].nY, "  ", 0);
			g_text1[i].nX += 1;
		}

		for (int i = 0; i < _countof(g_text1); i++) {
			if (g_text1[i].nX >= 0)
				WriteChar(g_text1[i].nX, g_text1[i].nY, "■", FG_GREEN);
		}

		Sleep(5);
	}
}



void prnStartText2(void)
{
	while (g_text2[_countof(g_text2) - 1].nX < 33) {
		for (int i = 0; i < _countof(g_text2); i++) {
			if (g_text2[i].nX >= 0)
				WriteChar(g_text2[i].nX, g_text2[i].nY, "  ", 0);
			g_text2[i].nX += 1;
		}

		for (int i = 0; i < _countof(g_text2); i++) {
			if (g_text2[i].nX >= 0)
				WriteChar(g_text2[i].nX, g_text2[i].nY, "■", FG_GREEN);
		}

		Sleep(5);
	}
}

void WelcomeAnim(void)
{
	prnStartTank();
	prnStartText1();
	prnStartText2();
}

//返回主菜单时显示
void ShowStillText(void)
{
	for (int i = 0; i < _countof(g_text1); i++) {
		if (g_text1[i].nX >= 0)
			WriteChar(g_text1[i].nX, g_text1[i].nY, "■", FG_GREEN);
	}

	for (int i = 0; i < _countof(g_text2); i++) {
		if (g_text2[i].nX >= 0)
			WriteChar(g_text2[i].nX, g_text2[i].nY, "■", FG_GREEN);
	}
}
