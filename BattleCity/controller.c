#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"

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

void UpdateMapPoint(ObjType eType, uint nX, uint nY)
{
	g_Map[nY][nX] = eType;
}

Tank *CreateTank(TankType eType, uint nX, uint nY, Direction eDrt)
{
	if (eType != SIGN_TANK_PA && eType != SIGN_TANK_PB 
		&& eType != SIGN_TANK_E0 && eType != SIGN_TANK_E1)
		return NULL;

	Tank *pTank = (Tank *)calloc(1, sizeof(Tank));
	pTank->eType = eType;
	pTank->nX = nX;
	pTank->nY = nY;
	pTank->eDrt = eDrt;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (g_TankStatus[eDrt][i][j] == 1) {
				UpdateMapPoint(eType, pTank->nX + j, pTank->nY + i);
			}
		}
	}

	return pTank;
}

void WipeTank(Tank *pTank)
{
	if (pTank == NULL)
		return;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			UpdateMapPoint(SIGN_EMPTY, pTank->nX + j, pTank->nY + i);
			WriteChar(pTank->nX + j, pTank->nY + i, "  ", 0);
		}
	}
}

void ShowTank(Tank *pTank)
{
	if (pTank == NULL)
		return;

	WORD wAttr = 0;
	switch (pTank->eType)
	{
	case SIGN_TANK_PA:
		wAttr = FG_LIGHTTURQUOISE;
		break;
	default:
		break;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			UpdateMapPoint(SIGN_TANK_PA, pTank->nX + j, pTank->nY + i);
			if (g_TankStatus[pTank->eDrt][i][j] == 1)
				WriteChar(pTank->nX + j, pTank->nY + i, "■", wAttr);
		}
	}
}

void MoveTank(Tank *pTank, Direction eDrt)
{
	if (pTank == NULL)
		return;

	if (pTank->eDrt != eDrt) {
		WipeTank(pTank);
		pTank->eDrt = eDrt;
		ShowTank(pTank);
		return;
	}

	switch (pTank->eDrt)
	{
	case DRT_UP:
		if (pTank->nY - 1 <= 0)
			return;
		WipeTank(pTank);
		pTank->nY -= 1;
		break;
	case DRT_DOWN:
		if (pTank->nY + 3 >= 39)
			return;
		WipeTank(pTank);
		pTank->nY += 1;
		break;
	case DRT_LEFT:
		if (pTank->nX - 1 <= 0)
			return;
		WipeTank(pTank);
		pTank->nX -= 1;
		break;
	case DRT_RIGHT:
		if (pTank->nX + 3 >= 39)
			return;
		WipeTank(pTank);
		pTank->nX += 1;
		break;
	}

	ShowTank(pTank);
}

int GetBullet(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (g_BulletBox[i].bulValid)
			return g_BulletBox[i].nBulletID;
	}
}

void WipeBullet(int nBulletID)
{
	UpdateMapPoint(SIGN_EMPTY, g_BulletBox[nBulletID].nX, g_BulletBox[nBulletID].nY);
	WriteChar(g_BulletBox[nBulletID].nX, g_BulletBox[nBulletID].nY, "  ", 0);
}

void ShowBullet(int nBulletID)
{
	UpdateMapPoint(SIGN_BULLET, g_BulletBox[nBulletID].nX, g_BulletBox[nBulletID].nY);
	WriteChar(g_BulletBox[nBulletID].nX, g_BulletBox[nBulletID].nY, "●", FG_LIGHTYELLOW);
}


void MoveBullets()
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (!g_BulletBox[i].bulValid) {
			WipeBullet(i);
			switch (g_BulletBox[i].eDrt)
			{
			case DRT_UP:
				if (g_BulletBox[i].nY - 1 <= 0) {
					g_BulletBox[i].bulValid = 1;
					continue;
				}
				g_BulletBox[i].nY -= 1;
				break;
			case DRT_DOWN:
				if (g_BulletBox[i].nY + 1 >= 39) {
					g_BulletBox[i].bulValid = 1;
					continue;
				}
				g_BulletBox[i].nY += 1;
				break;
			case DRT_LEFT:
				if (g_BulletBox[i].nX - 1 <= 0) {
					g_BulletBox[i].bulValid = 1;
					continue;
				}
				g_BulletBox[i].nX -= 1;
				break;
			case DRT_RIGHT:
				if (g_BulletBox[i].nX + 1 >= 39) {
					g_BulletBox[i].bulValid = 1;
					continue;
				}
				g_BulletBox[i].nX += 1;
				break;
			}

			ShowBullet(i);
		}
	}
}

void FireBullet(Tank *pTank)
{
	if (pTank == NULL)
		return;

	int nBulletID = GetBullet();
	g_BulletBox[nBulletID].eDrt = pTank->eDrt;

	switch (pTank->eDrt)
	{
	case DRT_UP:
		g_BulletBox[nBulletID].nX = pTank->nX + 1;
		g_BulletBox[nBulletID].nY = pTank->nY - 1;
		break;
	case DRT_DOWN:
		g_BulletBox[nBulletID].nX = pTank->nX + 1;
		g_BulletBox[nBulletID].nY = pTank->nY + 3;
		break;
	case DRT_LEFT:
		g_BulletBox[nBulletID].nX = pTank->nX - 1;
		g_BulletBox[nBulletID].nY = pTank->nY + 1;
		break;
	case DRT_RIGHT:
		g_BulletBox[nBulletID].nX = pTank->nX + 3;
		g_BulletBox[nBulletID].nY = pTank->nY + 1;
		break;
	}

	ShowBullet(nBulletID);
	g_BulletBox[nBulletID].bulValid = 0;
}

void DestroyTank(Tank *pTank)
{
	if (pTank != NULL) {
		WipeTank(pTank);
		free(pTank);
		pTank = NULL;
	}
}

Bullet *InitBulletBox(void)
{
	Bullet *bulletBox = (Bullet *)calloc(BOX_CAPACITY, sizeof(Bullet));
	for (int i = 0; i < BOX_CAPACITY; i++) {
		bulletBox[i].nBulletID = i;
		bulletBox[i].bulValid = 1;
		bulletBox[i].eDrt = DRT_NONE;
		bulletBox[i].eType = SIGN_BULLET;
		bulletBox[i].nX = -2;
		bulletBox[i].nY = -2;
	}

	return bulletBox;
}

void DestroyBulletBox(Bullet *pBulletBox)
{
	if (pBulletBox != NULL) {
		free(pBulletBox);
		pBulletBox = NULL;
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
