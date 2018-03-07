#include <fstream>

#include "CGameMap.h"
#include "Data.h"
#include "DrawTool.h"

using namespace std;

CGameMap::CGameMap() : m_nLevel(1), m_bUseDefault(false)
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
		if (!m_bUseDefault && isFileExist(MAP_FILE1_NEW))
			loadFile(MAP_FILE1_NEW);
		else
			loadFile(MAP_FILE1);
		break;
	case 2:
		if (!m_bUseDefault && isFileExist(MAP_FILE2_NEW))
			loadFile(MAP_FILE2_NEW);
		else
			loadFile(MAP_FILE2);
		break;
	case 3:
		if (!m_bUseDefault && isFileExist(MAP_FILE3_NEW))
			loadFile(MAP_FILE3_NEW);
		else
			loadFile(MAP_FILE3);
		break;
	}
	m_bUseDefault = false;
}

void CGameMap::drawMap()
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			//if (m_nArrMap[i][j] != SIGN_EMPTY)
				DrawTool::drawPattern(j, i, m_nArrMap[i][j]);
		}
	}
}

void CGameMap::editMap()
{
	loadMap();
	drawMap();

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	showMapEditorPanel();

	m_nEditNodeType = SIGN_WALL1;
	while (true) {
		updateEditorInfo();
		// �ȴ��¼�
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		// ��������¼�
		if (stcRecord.EventType == MOUSE_EVENT) {
			if (editMapMouseEventProc(stcRecord.Event.MouseEvent))
				break;
		}
	}
}

bool CGameMap::editMapMouseEventProc(MOUSE_EVENT_RECORD mer)
{
	bool bRet = false;
	int nX = mer.dwMousePosition.X / 2;
	int nY = mer.dwMousePosition.Y;

	switch (mer.dwEventFlags)
	{
	case 0:
		if (nY == 1 && nX == 46) { //�л�Ϊ��ǽ
			m_nEditNodeType = SIGN_WALL0;
		}
		else if (nY == 3 && nX == 46) { //�л�Ϊˮ��ǽ
			m_nEditNodeType = SIGN_WALL1;
		}
		else if (nY == 5 && nX == 46) {//�л�Ϊ����
			m_nEditNodeType = SIGN_RIVER; 
		}
		else if (nY == 7 && nX == 46) { //�л�Ϊ�ݵ�
			m_nEditNodeType = SIGN_GRASS;
		}
		else if (nY == 11) {
			if (nX >= 42 && nX <= 44) { //��ť"�ؿ�1"
				if (m_nLevel == 1)
					return false;
				m_nLevel = 1;
				loadMap();
				drawMap();
			}
			else if (nX >= 46 && nX <= 48) { //��ť"�ؿ�2"
				if (m_nLevel == 2)
					return false;
				m_nLevel = 2;
				loadMap();
				drawMap();
			}
			else if (nX >= 50 && nX <= 52) { //��ť"�ؿ�3"
				if (m_nLevel == 3)
					return false;
				m_nLevel = 3;
				loadMap();
				drawMap();
			}
		}
		else if (nY == 13 && nX >= 42 && nX <= 45) { //��ť"�ָ�Ĭ��"
			m_bUseDefault = true;
			loadMap();
			drawMap();
		}
		else if (nY == 15 && nX >= 42 && nX <= 45) { //��ť"һ�����"
			clearEditableArea();
		}
		else if (nY == 17 && nX >= 42 && nX <= 45) { //��ť"�����ͼ"
			saveMapToFile();
		}
		else if (nY == 19 && nX >= 42 && nX <= 45) { //��ť"����"
			bRet = true;
			goto END;
		}

		break;
	}

	//ֻ����ָ������༭��ͼ
	if (nX >= 1 && nX <= 38 && nY >= 4 && nY <= 33) {
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //����������
			DrawTool::drawPattern(nX, nY, m_nEditNodeType);
			m_nArrMap[nY][nX] = m_nEditNodeType;
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) { //��������Ҽ�
			DrawTool::drawPattern(nX, nY, SIGN_EMPTY);
			m_nArrMap[nY][nX] = SIGN_EMPTY;
		}
	}

END:
	return bRet;
}

void CGameMap::showMapEditorPanel()
{
	DrawTool::drawText(42, 1, "�֡�ǽ", FG_LIGHTTURQUOISE);
	DrawTool::drawPattern(46, 1, SIGN_WALL0);
	DrawTool::drawText(42, 3, "ˮ��ǽ", FG_LIGHTTURQUOISE);
	DrawTool::drawPattern(46, 3, SIGN_WALL1);
	DrawTool::drawText(42, 5, "�ӡ���", FG_LIGHTTURQUOISE);
	DrawTool::drawPattern(46, 5, SIGN_RIVER);
	DrawTool::drawText(42, 7, "�ݡ���", FG_LIGHTTURQUOISE);
	DrawTool::drawPattern(46, 7, SIGN_GRASS);

	DrawTool::drawText(42, 9, "��ǰ�ؿ�: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(42, 11, "�ؿ�1", BG_LIGHTYELLOW);
	DrawTool::drawText(46, 11, "�ؿ�2", BG_LIGHTYELLOW);
	DrawTool::drawText(50, 11, "�ؿ�3", BG_LIGHTYELLOW);
	DrawTool::drawText(42, 13, "�ָ�Ĭ��", BG_LIGHTGREEN);
	DrawTool::drawText(42, 15, "һ�����", BG_LIGHTGREEN);
	DrawTool::drawText(42, 17, "�����ͼ", BG_LIGHTGREEN);
	DrawTool::drawText(42, 19, "��������", BG_LIGHTGREEN);
}

void CGameMap::updateEditorInfo()
{
	char buf[16] = { 0 };
	sprintf_s(buf, _countof(buf), "%d", m_nLevel);
	DrawTool::drawText(47, 9, buf, FG_LIGHTTURQUOISE);
}

void CGameMap::clearEditableArea()
{
	for (int x = 1; x <= 38; x++) {
		for (int y = 4; y <= 33; y++) {
			m_nArrMap[y][x] = SIGN_EMPTY;
			DrawTool::drawPattern(x, y, SIGN_EMPTY);
		}
	}
}

void CGameMap::saveMapToFile()
{
	const char *pszFile = nullptr;
	char buf[64] = { 0 };

	if (m_nLevel == 1)
		pszFile = MAP_FILE1_NEW;
	else if (m_nLevel == 2)
		pszFile = MAP_FILE2_NEW;
	else if (m_nLevel == 3)
		pszFile = MAP_FILE3_NEW;

	fstream outFile(pszFile, ios::in | ios::out | ios::trunc);
	for (int x = 0; x < 40; x++) {
		for (int y = 0; y < 40; y++) {
			outFile.put(m_nArrMap[x][y] + 48);
		}
		outFile.put('\n');
	}
	outFile.close();
}

bool CGameMap::isFileExist(const char *pszFileName)
{
	fstream file(pszFileName);
	return !file ? false : true;
}

void CGameMap::loadFile(const char *pszFileName)
{
	char buf[64] = { 0 };
	fstream infile(pszFileName);

	for (int i = 0; i < 40; i++) {
		infile.getline(buf, _countof(buf));
		for (int j = 0; j < 40; j++) {
			m_nArrMap[i][j] = buf[j] - 48;
		}
	}

	infile.close();
}
