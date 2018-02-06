#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"
#include "map.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

#include <stdlib.h>
#include <conio.h>
#include <time.h>

void ShowMenu(void)
{
	int bExit = 0;

	MenuItem mainMenu[4] = {
		{ KEY_1, StartGame, "1. 开 始 游 戏" },
		{ KEY_2, LoadGame, "2. 继 续 游 戏" },
		{ KEY_3, EditMap, "3. 编 辑 地 图" },
		{ KEY_4, ExitGame, "4. 退 出 游 戏" }
	};

	while (!bExit) {
		ShowStillText();
		for (int i = 0; i < _countof(mainMenu); i++) {
			WriteChar(20, 22 + 2 * i, mainMenu[i].szText, FG_LIGHTGREEN);
		}

		while (1) {
			if (_kbhit()) {
				int key = _getch();
				if (key >= KEY_1 && key <= KEY_4) {
					bExit = mainMenu[key - 48 - 1].func();
					break;
				}
			}

			Sleep(30);
		}
	}
}

void PlayGame_KeyDetect(Tank *pTank)
{
	if (pTank->bDead)
		return;

	switch (pTank->eType)
	{
	case SIGN_TANK_PA:
		if (KEYDOWN('h') || KEYDOWN('H'))
			MoveTank(pTank, DRT_LEFT);
		else if (KEYDOWN('l') || KEYDOWN('L'))
			MoveTank(pTank, DRT_RIGHT);
		else if (KEYDOWN('k') || KEYDOWN('K'))
			MoveTank(pTank, DRT_UP);
		else if (KEYDOWN('j') || KEYDOWN('J'))
			MoveTank(pTank, DRT_DOWN);
		else if (KEYDOWN('a') || KEYDOWN('A'))
			FireBullet(pTank);

		break;
	default:
		break;
	}
}

void GoRun(void)
{
	g_startTime = clock();

	ShowMap();
	ShowTank(g_pTankA);
	//ShowTank(g_pTankB);
	ShowEnemies(g_pEnemies);

	while (1) {
		for (int i = 1; i < 39; i++) {
			for (int j = 1; j < 39; j++) {
				switch (g_Map[i][j])
				{
				case SIGN_GRASS:
					WriteChar(j, i, "▓", FG_GREEN);
					break;
				}
			}
		}

		PlayGame_KeyDetect(g_pTankA);
		MoveBullets();
		RandomMoveEnemies();

		Sleep(50);
	}
}

int StartGame(void)
{
	system("cls");

	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
	InitDefaultMap();

	GoRun();
	return 0;
}

int LoadGame(void)
{
	return 0;
}

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

void DrawMapMouseEventProc(MOUSE_EVENT_RECORD mer)
{
	int nX = mer.dwMousePosition.X / 2;
	int nY = mer.dwMousePosition.Y;

	switch (mer.dwEventFlags)
	{
	case 0:
		if (nX == 46) {
			if (nY == 1)
				g_eNodeType = SIGN_WALL0;
			else if (nY == 3)
				g_eNodeType = SIGN_WALL1;
			else if (nY == 5)
				g_eNodeType = SIGN_RIVER;
			else if (nY == 7)
				g_eNodeType = SIGN_GRASS;
		}

		if (nX >= 1 && nX <= 38 && nY >= 1 && nY <= 38) {
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

int EditMap(void)
{
	system("cls");

	ClearAllBarrier();
	ShowOnlyFourWall0();

	if (DrawMapResult()) {
		g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
		//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
		g_pBulletBox = InitBulletBox();
		g_pEnemies = InitEnemies();

		GoRun();
	}
	else {
		ClearAllBarrier();
		system("cls");
	}

	return 0;
}

int ExitGame(void)
{
	FreeGameRes();
	return 1;
}


void FreeGameRes(void)
{
	DestroyObject(g_pTankA);
	DestroyObject(g_pTankB);
	DestroyObject(g_pEnemies);
	DestroyObject(g_pBulletBox);
}

void LaunchGame(void)
{
	//初始化窗口
	SetWindowSize("Battle City", 100, 40);

	//播放动画
	WelcomeAnim();

	//显示菜单
	ShowMenu();
}
