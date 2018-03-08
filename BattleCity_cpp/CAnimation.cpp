#include "CAnimation.h"
#include "DrawTool.h"
#include "CTank.h"
#include "Data.h"

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::welcomeAnim()
{
	prnStartTank();
	prnStartText1();
	prnStartText2();
}

void CAnimation::showStillText()
{
	for (int i = 0; i < _countof(m_text1); i++) {
		if (m_text1[i].nX >= 0)
			DrawTool::drawText(m_text1[i].nX, m_text1[i].nY, "¡ö", FG_GREEN);
	}

	for (int i = 0; i < _countof(m_text2); i++) {
		if (m_text2[i].nX >= 0)
			DrawTool::drawText(m_text2[i].nX, m_text2[i].nY, "¡ö", FG_GREEN);
	}
}

void CAnimation::prnStartTank()
{
	int dx1 = 0, dx2 = 0;
	int nBullets[2] = { 1, 1 };   //Á½·¢×Óµ¯

	while (dx1 < 58) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (CTank::m_nTankShape[DRT_RIGHT][i][j] == 1) {
					if (j + dx1 >= 58)
						DrawTool::drawPattern(j + dx1, i + 10, SIGN_EMPTY);
					else
						DrawTool::drawPattern(j + dx1, i + 10, SIGN_TANK_PA);
				}
			}
		}

		for (int i = 0; i < 2; i++) {
			if (nBullets[i] == 1) {
				if (i + 3 + dx2 >= 58) {
					nBullets[i] = 0;
					DrawTool::drawPattern(i + 3 + dx2, 1 + 10, SIGN_EMPTY);
				}
				else {
					DrawTool::drawPattern(i + 3 + dx2, 1 + 10, SIGN_BULLET0);
				}
			}
		}

		Sleep(30);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (CTank::m_nTankShape[DRT_RIGHT][i][j] == 1)
					DrawTool::drawPattern(j + dx1, i + 10, SIGN_EMPTY);
			}
		}

		for (int i = 0; i < 2; i++) {
			if (nBullets[i] == 1)
				DrawTool::drawPattern(i + 3 + dx2, 1 + 10, SIGN_EMPTY);
		}

		dx1++;
		dx2 += 3;
	}
}

void CAnimation::prnStartText1()
{
	while (m_text1[_countof(m_text1) - 1].nX < 43) {
		for (int i = 0; i < _countof(m_text1); i++) {
			if (m_text1[i].nX >= 0)
				DrawTool::drawPattern(m_text1[i].nX, m_text1[i].nY, SIGN_EMPTY);
			m_text1[i].nX += 1;
		}

		for (int i = 0; i < _countof(m_text1); i++) {
			if (m_text1[i].nX >= 0)
				DrawTool::drawText(m_text1[i].nX, m_text1[i].nY, "¡ö", FG_GREEN);
		}

		Sleep(5);
	}
}

void CAnimation::prnStartText2()
{
	while (m_text2[_countof(m_text2) - 1].nX < 36) {
		for (int i = 0; i < _countof(m_text2); i++) {
			if (m_text2[i].nX >= 0)
				DrawTool::drawPattern(m_text2[i].nX, m_text2[i].nY, SIGN_EMPTY);
			m_text2[i].nX += 1;
		}

		for (int i = 0; i < _countof(m_text2); i++) {
			if (m_text2[i].nX >= 0)
				DrawTool::drawText(m_text2[i].nX, m_text2[i].nY, "¡ö", FG_GREEN);
		}

		Sleep(5);
	}
}

#define DX      -32
#define DY      -5
Point CAnimation::m_text1[60] = {
		{ 0 + DX, 12 + DY },
		{ 0 + DX, 13 + DY },
		{ 0 + DX, 14 + DY },
		{ 0 + DX, 15 + DY },
		{ 0 + DX, 16 + DY },
		{ 1 + DX, 12 + DY },
		{ 1 + DX, 14 + DY },
		{ 1 + DX, 16 + DY },
		{ 2 + DX, 12 + DY },
		{ 2 + DX, 13 + DY },
		{ 2 + DX, 15 + DY },
		{ 2 + DX, 16 + DY }, //B

		{ 4 + DX, 16 + DY },
		{ 5 + DX, 15 + DY },
		{ 6 + DX, 14 + DY },
		{ 7 + DX, 13 + DY },
		{ 7 + DX, 14 + DY },
		{ 8 + DX, 12 + DY },
		{ 8 + DX, 14 + DY },
		{ 9 + DX, 13 + DY },
		{ 9 + DX, 14 + DY },
		{ 10 + DX, 14 + DY },
		{ 11 + DX, 15 + DY },
		{ 12 + DX, 16 + DY }, //A

		{ 14 + DX, 12 + DY },
		{ 15 + DX, 12 + DY },
		{ 16 + DX, 12 + DY },
		{ 16 + DX, 13 + DY },
		{ 16 + DX, 14 + DY },
		{ 16 + DX, 15 + DY },
		{ 16 + DX, 16 + DY },
		{ 17 + DX, 12 + DY },
		{ 18 + DX, 12 + DY }, //T

		{ 20 + DX, 12 + DY },
		{ 21 + DX, 12 + DY },
		{ 22 + DX, 12 + DY },
		{ 22 + DX, 13 + DY },
		{ 22 + DX, 14 + DY },
		{ 22 + DX, 15 + DY },
		{ 22 + DX, 16 + DY },
		{ 23 + DX, 12 + DY },
		{ 24 + DX, 12 + DY }, //T

		{ 26 + DX, 12 + DY },
		{ 26 + DX, 13 + DY },
		{ 26 + DX, 14 + DY },
		{ 26 + DX, 15 + DY },
		{ 26 + DX, 16 + DY },
		{ 27 + DX, 16 + DY },
		{ 28 + DX, 16 + DY }, //L

		{ 30 + DX, 12 + DY },
		{ 30 + DX, 13 + DY },
		{ 30 + DX, 14 + DY },
		{ 30 + DX, 15 + DY },
		{ 30 + DX, 16 + DY },
		{ 31 + DX, 12 + DY },
		{ 31 + DX, 14 + DY },
		{ 31 + DX, 16 + DY },
		{ 32 + DX, 12 + DY },
		{ 32 + DX, 14 + DY },
		{ 32 + DX, 16 + DY }, //E
};
#undef DX
#undef DY

#define DX    -20
#define DY    -3
Point CAnimation::m_text2[32] = {
		{ 0 + DX, 18 + DY },
		{ 0 + DX, 19 + DY },
		{ 0 + DX, 20 + DY },
		{ 1 + DX, 17 + DY },
		{ 1 + DX, 21 + DY },
		{ 2 + DX, 17 + DY },
		{ 2 + DX, 21 + DY }, //C

		{ 5 + DX, 17 + DY },
		{ 5 + DX, 21 + DY },
		{ 6 + DX, 17 + DY },
		{ 6 + DX, 18 + DY },
		{ 6 + DX, 19 + DY },
		{ 6 + DX, 20 + DY },
		{ 6 + DX, 21 + DY },
		{ 7 + DX, 17 + DY },
		{ 7 + DX, 21 + DY }, //I

		{ 9 + DX, 17 + DY },
		{ 10 + DX, 17 + DY },
		{ 11 + DX, 17 + DY },
		{ 11 + DX, 18 + DY },
		{ 11 + DX, 19 + DY },
		{ 11 + DX, 20 + DY },
		{ 11 + DX, 21 + DY },
		{ 12 + DX, 17 + DY },
		{ 13 + DX, 17 + DY }, //T

		{ 15 + DX, 17 + DY },
		{ 16 + DX, 18 + DY },
		{ 17 + DX, 19 + DY },
		{ 17 + DX, 20 + DY },
		{ 17 + DX, 21 + DY },
		{ 18 + DX, 18 + DY },
		{ 19 + DX, 17 + DY }, //Y
};
#undef DX
#undef DY
