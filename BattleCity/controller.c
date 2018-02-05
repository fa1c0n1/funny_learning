#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"
#include "map.h"
#include "player.h"
#include "bullet.h"

#include <conio.h>

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

void StartGame(void)
{
	system("cls");

	Tank *pTankA = CreateTank(SIGN_TANK_PA, 12, 36, DRT_UP);

	//显示四面固定的贴墙
	for (int i = 0; i < 40; i++) {
		if (i == 0 || i == 39) {
			for (int j = 0; j < 40; j++) {
				WriteChar(j, i, "■", FG_HIGHWHITE);
			}
		}
		else {
			WriteChar(0, i, "■", FG_HIGHWHITE);
			WriteChar(39, i, "■", FG_HIGHWHITE);
		}
		Sleep(5);
	}

	ShowTank(pTankA);
	g_BulletBox = InitBulletBox();

	while (1) {

		/*for (int i = 1; i < 39; i++) {
			for (int j = 1; j < 39; j++) {
			switch (g_Map[i][j])
			{
			case SIGN_WALL0:
			WriteChar(j, i, "■", FG_HIGHWHITE);
			break;
			case SIGN_TANK_PA:
			WriteChar(j, i, "■", FG_LIGHTTURQUOISE);
			break;
			case SIGN_BULLET:
			WriteChar(j, i, "●", FG_LIGHTYELLOW);
			break;
			default:
			break;
			}
			}
			}*/

		PlayGame_MsgLoop(pTankA);

		MoveBullets();

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
