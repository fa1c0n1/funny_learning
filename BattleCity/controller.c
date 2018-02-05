#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"
#include "map.h"
#include "player.h"
#include "bullet.h"

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

Tank *InitEnemies()
{
	g_nEnNum = ENEMY_NMAX;
	Tank *pEnemies = (Tank *)calloc(ENEMY_NMAX, sizeof(Tank));
	for (int i = 0; i < ENEMY_NMAX; i++) {
		pEnemies[i].eDrt = DRT_DOWN;
		pEnemies[i].eType = SIGN_TANK_E0;
		pEnemies[i].nX = 5 + i * 5;
		pEnemies[i].nY = 1;
		pEnemies[i].bDead = 0;
	}

	return pEnemies;
}

void ShowEnemies(Tank *pEnemies)
{
	if (pEnemies == NULL)
		return;

	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!pEnemies[i].bDead) {
			ShowTank(pEnemies + i);
		}
	}
}

void RandomMoveEnemies()
{
	Direction drtArr[4] = { DRT_UP, DRT_DOWN, DRT_LEFT, DRT_RIGHT };
	srand(time(NULL));
	clock_t endTime;

	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!g_pEnemies[i].bDead) {
			endTime = clock();
			if ((endTime - g_startTime) % 150 + 150 > 200) {
				MoveTank(&g_pEnemies[i], drtArr[rand() % 4]);
			}
		}
	}
}

void DestroyEnemies(Tank *pEnemies)
{
	if (pEnemies != NULL) {
		free(pEnemies);
		pEnemies = NULL;
	}
}

void StartGame(void)
{
	system("cls");
	g_startTime = clock();

	ShowMap();

	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	ShowTank(g_pTankA);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
	ShowEnemies(g_pEnemies);

	while (1) {
		for (int i = 1; i < 39; i++) {
			for (int j = 1; j < 39; j++) {
				switch (g_Map[i][j])
				{
				/*case SIGN_WALL1: 
					WriteChar(j, i, "■", FG_YELLOW);               
					break;*/
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
	exit(0);
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
