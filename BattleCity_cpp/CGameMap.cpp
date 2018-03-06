#include <fstream>

#include "CGameMap.h"
#include "Data.h"
#include "DrawTool.h"

using namespace std;

CGameMap::CGameMap() : m_nLevel(1)
{
}


CGameMap::~CGameMap()
{
}

void CGameMap::setMapValue(int nX, int nY, int nVal)
{
	m_nArrMap[nY][nX] = nVal;
}

int CGameMap::getMapValue(int nX, int nY)
{
	return m_nArrMap[nY][nX];
}

void CGameMap::setLevel(int nLevel)
{
	m_nLevel = nLevel;
}

void CGameMap::loadMap()
{
	switch (m_nLevel)
	{
	case 1:
		loadFile(MAP_FILE1);
		break;
	case 2:
		loadFile(MAP_FILE2);
		break;
	case 3:
		loadFile(MAP_FILE3);
		break;
	}
}

void CGameMap::drawMap()
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			if (m_nArrMap[i][j] != SIGN_EMPTY)
				DrawTool::drawPattern(j, i, m_nArrMap[i][j]);
		}
	}
}

void CGameMap::loadFile(const char *szFileName)
{
	char buf[64] = { 0 };
	fstream infile(szFileName);

	for (int i = 0; i < 40; i++) {
		infile.getline(buf, _countof(buf));
		for (int j = 0; j < 40; j++) {
			m_nArrMap[i][j] = buf[j] - 48;
		}
	}

	infile.close();
}
