#include "draw_tool.h"
#include "animation.h"
#include "data.h"

void prnStartTank(void)
{
	int dx1 = 0, dx2 = 0;
	int nBullets[2] = { 1, 1 };   //Á½·¢×Óµ¯

	while (dx1 < 50) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (g_TankStatus[3][i][j] == 1) {
					if (j + dx1 >= 50)
						WriteChar(j + dx1, i + 10, "  ", 0);
					else
						WriteChar(j + dx1, i + 10, "¡ö", FG_LIGHTTURQUOISE);
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
					WriteChar(i + 3 + dx2, 1 + 10, "¡ñ", FG_LIGHTYELLOW);
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
	int dx = -32;
	int dy = -5;

	Point text[] = {
			{ 0 + dx, 12 + dy },
			{ 0 + dx, 13 + dy },
			{ 0 + dx, 14 + dy },
			{ 0 + dx, 15 + dy },
			{ 0 + dx, 16 + dy },
			{ 1 + dx, 12 + dy },
			{ 1 + dx, 14 + dy },
			{ 1 + dx, 16 + dy },
			{ 2 + dx, 12 + dy },
			{ 2 + dx, 13 + dy },
			{ 2 + dx, 15 + dy },
			{ 2 + dx, 16 + dy }, //B

			{ 4 + dx, 16 + dy },
			{ 5 + dx, 15 + dy },
			{ 6 + dx, 14 + dy },
			{ 7 + dx, 13 + dy },
			{ 7 + dx, 14 + dy },
			{ 8 + dx, 12 + dy },
			{ 8 + dx, 14 + dy },
			{ 9 + dx, 13 + dy },
			{ 9 + dx, 14 + dy },
			{ 10 + dx, 14 + dy },
			{ 11 + dx, 15 + dy },
			{ 12 + dx, 16 + dy }, //A
		
			{ 14 + dx, 12 + dy },
			{ 15 + dx, 12 + dy },
			{ 16 + dx, 12 + dy },
			{ 16 + dx, 13 + dy },
			{ 16 + dx, 14 + dy },
			{ 16 + dx, 15 + dy },
			{ 16 + dx, 16 + dy },
			{ 17 + dx, 12 + dy },
			{ 18 + dx, 12 + dy }, //T

			{ 20 + dx, 12 + dy },
			{ 21 + dx, 12 + dy },
			{ 22 + dx, 12 + dy },
			{ 22 + dx, 13 + dy },
			{ 22 + dx, 14 + dy },
			{ 22 + dx, 15 + dy },
			{ 22 + dx, 16 + dy }, 
			{ 23 + dx, 12 + dy },
			{ 24 + dx, 12 + dy }, //T

			{ 26 + dx, 12 + dy },
			{ 26 + dx, 13 + dy },
			{ 26 + dx, 14 + dy },
			{ 26 + dx, 15 + dy },
			{ 26 + dx, 16 + dy },
			{ 27 + dx, 16 + dy },
			{ 28 + dx, 16 + dy }, //L

			{ 30 + dx, 12 + dy },
			{ 30 + dx, 13 + dy },
			{ 30 + dx, 14 + dy },
			{ 30 + dx, 15 + dy },
			{ 30 + dx, 16 + dy },
			{ 31 + dx, 12 + dy },
			{ 31 + dx, 14 + dy },
			{ 31 + dx, 16 + dy },
			{ 32 + dx, 12 + dy },
			{ 32 + dx, 14 + dy },
			{ 32 + dx, 16 + dy }, //E
	};

	while (text[_countof(text) - 1].nX < 40) {
		for (int i = 0; i < _countof(text); i++) {
			if (text[i].nX >= 0)
				WriteChar(text[i].nX, text[i].nY, "  ", 0);
			text[i].nX += 1;
		}

		for (int i = 0; i < _countof(text); i++) {
			if (text[i].nX >= 0)
				WriteChar(text[i].nX, text[i].nY, "¡ö", FG_GREEN);
		}

		Sleep(5);
	}
}

void prnStartText2(void)
{
	int dx = -20;
	int dy = -3;
	
	Point text[] = {
			{ 0 + dx, 18 + dy },
			{ 0 + dx, 19 + dy },
			{ 0 + dx, 20 + dy },
			{ 1 + dx, 17 + dy },
			{ 1 + dx, 21 + dy },
			{ 2 + dx, 17 + dy },
			{ 2 + dx, 21 + dy }, //C

			{ 5 + dx, 17 + dy },
			{ 5 + dx, 21 + dy },
			{ 6 + dx, 17 + dy }, 
			{ 6 + dx, 18 + dy }, 
			{ 6 + dx, 19 + dy }, 
			{ 6 + dx, 20 + dy }, 
			{ 6 + dx, 21 + dy }, 
			{ 7 + dx, 17 + dy },
			{ 7 + dx, 21 + dy }, //I

			{ 9 + dx, 17 + dy },
			{ 10 + dx, 17 + dy },
			{ 11 + dx, 17 + dy },
			{ 11 + dx, 18 + dy },
			{ 11 + dx, 19 + dy },
			{ 11 + dx, 20 + dy },
			{ 11 + dx, 21 + dy },
			{ 12 + dx, 17 + dy }, 
			{ 13 + dx, 17 + dy }, //T

			{ 15 + dx, 17 + dy },
			{ 16 + dx, 18 + dy },
			{ 17 + dx, 19 + dy },
			{ 17 + dx, 20 + dy },
			{ 17 + dx, 21 + dy },
			{ 18 + dx, 18 + dy },
			{ 19 + dx, 17 + dy }, //Y
	};

	while (text[_countof(text) - 1].nX < 33) {
		for (int i = 0; i < _countof(text); i++) {
			if (text[i].nX >= 0)
				WriteChar(text[i].nX, text[i].nY, "  ", 0);
			text[i].nX += 1;
		}

		for (int i = 0; i < _countof(text); i++) {
			if (text[i].nX >= 0)
				WriteChar(text[i].nX, text[i].nY, "¡ö", FG_GREEN);
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