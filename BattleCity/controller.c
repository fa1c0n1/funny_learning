#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"
#include "map.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "pillbox.h"

#include <stdlib.h>
#include <conio.h>
#include <time.h>

//显示主菜单
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

//显示子菜单
int ShowSubMenu(void)
{
	int nRet = 0;

	MenuItem subMenu[3] = {
		{ KEY_1, GoOnGame, "1.继续游戏" },
		{ KEY_2, SaveGame, "2.保存游戏" },
		{ KEY_3, RetMainMenu, "3.返　　回" },
	};

	for (int i = 0; i < _countof(subMenu); i++) {
		WriteChar(42, 25 + 2 * i, subMenu[i].szText, FG_LIGHTGREEN);
	}

	while (1) {
		if (_kbhit()) {
			int key = _getch();
			if (key >= KEY_1 && key <= KEY_3) {
				nRet = subMenu[key - 48 - 1].func();
				if (nRet != 2)
					break;
			}
		}

		Sleep(30);
	}

	for (int i = 0; i < _countof(subMenu); i++) {
		WriteChar(42, 25 + 2 * i, "         ", 0);
	}

	return nRet;
}

//子菜单：继续游戏
int GoOnGame(void)
{
	return 1;
}

//子菜单：保存游戏
int SaveGame(void)
{
	//TODO:
	return 2;
}

//子菜单：返回主菜单
int RetMainMenu(void)
{
	return 3;
}

//进入游戏的主逻辑的循环
void GoRun(void)
{
	g_startTime = clock();
	int nSubRet = 0;

	ShowMap();
	ShowPillbox(g_pPillbox);
	ShowTank(g_pTankA);
	//ShowTank(g_pTankB);
	ShowEnemies(g_pEnemies);

	while (1) {
		if (!g_pTankA->bDead && !g_pPillbox->bDead) {
			//按键检测
			switch (g_pTankA->eType)
			{
			case SIGN_TANK_PA:
				if (KEYDOWN('a') || KEYDOWN('A'))
					MoveTank(g_pTankA, DRT_LEFT);
				else if (KEYDOWN('d') || KEYDOWN('D'))
					MoveTank(g_pTankA, DRT_RIGHT);
				else if (KEYDOWN('w') || KEYDOWN('W'))
					MoveTank(g_pTankA, DRT_UP);
				else if (KEYDOWN('s') || KEYDOWN('S'))
					MoveTank(g_pTankA, DRT_DOWN);
				else if (KEYDOWN('j') || KEYDOWN('J'))
					FireBullet(g_pTankA);
				else if (KEYDOWN('')) {
					nSubRet = ShowSubMenu();
					if (nSubRet == 3) {//3表示要返回到主菜单
						ClearAllBarrier();
						system("cls");
						goto END;
					}
				}

				break;
			default:
				break;
			}
		}
		else {
			ShowFailedNotice();
			Sleep(3000);
			system("cls");
			goto END;
		}

		MoveBullets();
		RandomMoveEnemies();

		Sleep(50);
	}

END:
	return;
}

void ShowFailedNotice(void)
{
	WriteChar(7, 7, "━━━━━━━━━━━━━━━━━━━━━━", FG_LIGHTRED);
	WriteChar(7, 8, "┃               GAME OVER                ┃", FG_LIGHTRED);
	WriteChar(7, 9, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 10, "┃       胜败乃兵家常事, 请从头再来！     ┃", FG_LIGHTRED);
	WriteChar(7, 11, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 12, "┃━━━━━━━━━━━━━━━━━━━━┃", FG_LIGHTRED);
}

//初始化游戏角色(包括玩家、敌军和子弹)
void InitGameRoles(void)
{
	g_pPillbox = InitPillbox();
	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
}

//主菜单：开始游戏
int StartGame(void)
{
	system("cls");
	InitGameRoles();
	InitDefaultMap();
	GoRun();
	FreeGameRes();
	return 0;
}

//主菜单：继续游戏
int LoadGame(void)
{
	//TODO:
	return 0;
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

//主菜单：编辑地图
int EditMap(void)
{
	system("cls");

	ClearAllBarrier();
	ShowMap();

	if (DrawMapResult()) {
		InitGameRoles();
		GoRun();
		FreeGameRes();
	}
	else {
		ClearAllBarrier();
		system("cls");
	}

	return 0;
}

//主菜单：退出游戏
int ExitGame(void)
{
	FreeGameRes();
	return 1;
}


//释放游戏资源
void FreeGameRes(void)
{
	DestroyObject(&g_pTankA);
	//DestroyObject(&g_pTankB);
	DestroyObject(&g_pEnemies);
	DestroyObject(&g_pBulletBox);
	DestroyObject(&g_pPillbox);
}

//启动游戏(被main函数调用)
void LaunchGame(void)
{
	//初始化窗口
	SetWindowSize("Battle City", 100, 40);

	WelcomeAnim();
	ShowMenu();
}
