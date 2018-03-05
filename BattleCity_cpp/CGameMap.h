#pragma once

class CGameMap
{
public:
	CGameMap();
	~CGameMap();

	void setLevel(int nLevel);
	void loadMap();
	void setMapValue(int nX, int nY, int nVal);
	int getMapValue(int nX, int nY);
	void drawMap();

private:
	void loadFile(const char *szFileName);

	int m_nLevel;
	int m_nArrMap[40][40];
};

