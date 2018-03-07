#pragma once

#include <windows.h>

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
	void editMap();

private:
	bool editMapMouseEventProc(MOUSE_EVENT_RECORD mer);
	void showMapEditorPanel();
	void updateEditorInfo();
	void clearEditableArea();
	void saveMapToFile();
	bool isFileExist(const char *pszFileName);
	void loadFile(const char *pszFileName);

	bool m_bUseDefault;
	int m_nLevel;
	int m_nEditNodeType;
	int m_nArrMap[40][40];
};

