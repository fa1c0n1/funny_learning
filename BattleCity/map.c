#include "map.h"

void UpdateMapPoint(ObjType eType, uint nX, uint nY)
{
	g_Map[nY][nX] = eType;
}

//清除地图上所有的障碍物
void ClearAllBarrier(void)
{
	//恢复碉堡周围的水泥墙
	for (int i = 34; i <= 35; i++) {
		for (int j = 15; j <= 21; j++) {
			UpdateMapPoint(SIGN_WALL1, j, i);
		}
	}

	for (int i = 36; i <= 38; i++) {
		for (int j = 15; j <= 21; j++) {
			if (j < 17 || j > 19) {
				UpdateMapPoint(SIGN_WALL1, j, i);
			}
		}
	}

	for (int i = 1; i < 39; i++) {
		for (int j = 1; j < 39; j++) {
			if (j >= 15 && j <= 21) {
				if (i >= 34 && i <= 38)
					continue;
			}

			if (g_Map[i][j] != SIGN_EMPTY) {
				g_Map[i][j] = SIGN_EMPTY;
			}
		}
	}
}

//初始化默认地图
void InitDefaultMap(void)
{
	//布置水泥墙
	for (int i = 7; i <= 15; i++) {
		for (int j = 7; j <= 9; j++) {
			g_Map[i][j] = SIGN_WALL1;
		}
	}

	//布置钢墙
	for (int i = 21; i <= 22; i++) {
		for (int j = 5; j <= 7; j++) {
			g_Map[i][j] = SIGN_WALL0;
		}
	}
	
	//布置河流
	for (int i = 21; i <= 22; i++) {
		for (int j = 16; j <= 17; j++) {
			g_Map[i][j] = SIGN_RIVER;
		}
	}

	for (int i = 16; i <= 20; i++) {
		for (int j = 23; j <= 24; j++) {
			g_Map[i][j] = SIGN_RIVER;
		}
	}

	//布置草地
	for (int i = 26; i <= 28; i++) {
		for (int j = 19; j <= 25; j++) {
			g_Map[i][j] = SIGN_GRASS;
		}
	}
}

void ShowMap(void)
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			switch (g_Map[i][j])
			{
			case SIGN_WALL0:
				WriteChar(j, i, "■", FG_HIGHWHITE);
				break;
			case SIGN_WALL1:
				WriteChar(j, i, "■", FG_YELLOW);
				break;
			case SIGN_GRASS:
				WriteChar(j, i, "▓", FG_GREEN);
				break;
			case SIGN_RIVER:
				WriteChar(j, i, "▓", FG_LIGHTBLUE);
				break;
			default:
				WriteChar(j, i, "  ", 0);
				break;
			}
		}
	}
}

//释放游戏中分配的动态内存
void DestroyObject(Object **pObj)
{
	if (pObj == NULL)
		return;

	if (*pObj != NULL) {
		free(*pObj);
		*pObj = NULL;
	}
}

//编辑地图时的键盘检测处理
int DrawMapKeyEventProc(KEY_EVENT_RECORD ker)
{
	int bRet = 0;

	if (ker.bKeyDown) { //键盘按下
		CHAR keyVal = ker.uChar.AsciiChar;
		if (keyVal == 's') {
			bRet = 1;  //保存编辑并开始游戏
		}
		else if (keyVal == 'q') {
			bRet = 2; //放弃编辑并返回
		}
	}
	else {  //键盘弹起

	}

END:
	return bRet;
}

//编辑地图时的鼠标检测处理
void DrawMapMouseEventProc(MOUSE_EVENT_RECORD mer)
{
	int nX = mer.dwMousePosition.X / 2;
	int nY = mer.dwMousePosition.Y;

	switch (mer.dwEventFlags)
	{
	case 0:
		if (nX == 46) {  //点击障碍物切换区域
			if (nY == 1)
				g_eNodeType = SIGN_WALL0;
			else if (nY == 3)
				g_eNodeType = SIGN_WALL1;
			else if (nY == 5)
				g_eNodeType = SIGN_RIVER;
			else if (nY == 7)
				g_eNodeType = SIGN_GRASS;

			return;
		}

		if (nX >= 1 && nX <= 38 && nY >= 1 && nY <= 38) {
			if (nX >= 15 && nX <= 21) { //碉堡区域不能编辑
				if (nY >= 34 && nY <= 38)
					return;
			}

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //按下鼠标左键
				switch (g_eNodeType)
				{
				case SIGN_WALL0:
					WriteChar(nX, nY, "■", FG_HIGHWHITE);
					break;
				case SIGN_WALL1:
					WriteChar(nX, nY, "■", FG_YELLOW);
					break;
				case SIGN_RIVER:
					WriteChar(nX, nY, "▓", FG_LIGHTBLUE);
					break;
				case SIGN_GRASS:
					WriteChar(nX, nY, "▓", FG_GREEN);
					break;
				}

				g_Map[nY][nX] = g_eNodeType;
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) { //按下鼠标右键
				g_Map[nY][nX] = SIGN_EMPTY;
				WriteChar(nX, nY, "  ", 0);
			}
		}

		break;
	}
}

//编辑地图
int DrawMapResult(void)
{
	int bSave = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	WriteChar(42, 1, "钢　墙", FG_WHITE);
	WriteChar(46, 1, "■", FG_HIGHWHITE);
	WriteChar(42, 3, "水泥墙", FG_WHITE);
	WriteChar(46, 3, "■", FG_YELLOW);
	WriteChar(42, 5, "河　流", FG_WHITE);
	WriteChar(46, 5, "▓", FG_LIGHTBLUE);
	WriteChar(42, 7, "草　地", FG_WHITE);
	WriteChar(46, 7, "▓", FG_GREEN);

	while (1)
	{
		// 等待事件
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		// 处理事件
		if (stcRecord.EventType == KEY_EVENT)
		{
			// 键盘事件
			int bRet = DrawMapKeyEventProc(stcRecord.Event.KeyEvent);
			if (bRet) {
				WriteChar(42, 1, "      ", FG_WHITE);
				WriteChar(46, 1, "  ", FG_HIGHWHITE);
				WriteChar(42, 3, "      ", FG_WHITE);
				WriteChar(46, 3, "  ", FG_YELLOW);
				WriteChar(42, 5, "      ", FG_WHITE);
				WriteChar(46, 5, "   ", FG_LIGHTBLUE);
				WriteChar(42, 7, "      ", FG_WHITE);
				WriteChar(46, 7, "  ", FG_GREEN);

				bSave = (bRet == 1 ? 1 : 0);
				break;
			}
		}
		else if (stcRecord.EventType == MOUSE_EVENT) {
			DrawMapMouseEventProc(stcRecord.Event.MouseEvent);
		}
	}

	return bSave;
}