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
	ShowTODO();
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
			if (g_nEnNum <= 0) { //敌军被全灭
				ShowWinNotice();
				Sleep(3000);
				ClearAllBarrier();
				system("cls");
				goto END;
			}

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
		else { //玩家被灭或碉堡被击毁
			ClearAllBarrier();
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

//显示胜利提示
void ShowWinNotice(void)
{

	WriteChar(7, 7, "━━━━━━━━━━━━━━━━━━━━━━", FG_LIGHTRED);
	WriteChar(7, 8, "┃               YOU WIN                  ┃", FG_LIGHTRED);
	WriteChar(7, 9, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 10, "┃       恭喜你！战胜了敌人！！！         ┃", FG_LIGHTRED);
	WriteChar(7, 11, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 12, "┃━━━━━━━━━━━━━━━━━━━━┃", FG_LIGHTRED);
}

//显示失败提示
void ShowFailedNotice(void)
{
	WriteChar(7, 7, "━━━━━━━━━━━━━━━━━━━━━━", FG_LIGHTRED);
	WriteChar(7, 8, "┃               GAME OVER                ┃", FG_LIGHTRED);
	WriteChar(7, 9, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 10, "┃       胜败乃兵家常事, 请从头再来！     ┃", FG_LIGHTRED);
	WriteChar(7, 11, "┃                                        ┃", FG_LIGHTRED);
	WriteChar(7, 12, "┃━━━━━━━━━━━━━━━━━━━━┃", FG_LIGHTRED);
}

//显示功能未完成
void ShowTODO(void)
{
	WriteChar(41, 32, " ━━━━━━ ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "┃功能未完成┃", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "┃          ┃", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "┃ 敬请期待 ┃", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "┃__________┃", FG_LIGHTTURQUOISE);
	Sleep(1000);
	WriteChar(41, 32, "             ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "              ", FG_LIGHTTURQUOISE);
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
	ShowTODO();
	return 0;
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
