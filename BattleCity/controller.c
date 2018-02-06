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
	MenuItem mainMenu[4] = {
		{ KEY_1, StartGame, "1. 开 始 游 戏" },
		{ KEY_2, LoadGame, "2. 继 续 游 戏" },
		{ KEY_3, EditMap, "3. 编 辑 地 图" },
		{ KEY_4, ExitGame, "4. 退 出 游 戏" }
	};

	for (int i = 0; i < _countof(mainMenu); i++) {
		WriteChar(20, 22 + 2 * i, mainMenu[i].szText, FG_LIGHTGREEN);
	}

	while (1) {
		if (_kbhit()) {
			int key = _getch();
			if (key >= KEY_1 && key <= KEY_4)
				mainMenu[key - 48 - 1].func();
		}

		Sleep(30);
	}
}

void PlayGame_MsgLoop(Tank *pTank)
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

void StartGame(void)
{
	system("cls");
	g_startTime = clock();

	ShowMap();

	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
	ShowTank(g_pTankA);
	//ShowTank(g_pTankB);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
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

		PlayGame_MsgLoop(g_pTankA);
		MoveBullets();
		RandomMoveEnemies();

		Sleep(50);
	}
}

void LoadGame(void)
{

}

void EditMap(void)
{

}

void ExitGame(void)
{
	FreeGameRes();
	exit(0);
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
