#include "bullet.h"
#include "draw_tool.h"
#include "map.h"
#include "player.h"

#include <stdlib.h>

int GetBullet(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (g_pBulletBox[i].bulValid)
			return g_pBulletBox[i].nBulletID;
	}
}

void WipeBullet(int nBulletID)
{
	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	switch (g_Map[nTY][nTX])
	{
	case SIGN_GRASS:
	case SIGN_RIVER:
		break;
	default:
		UpdateMapPoint(SIGN_EMPTY, nTX, nTY);
		WriteChar(nTX, nTY, "  ", 0);
		break;
	}
}

void ShowBullet(int nBulletID)
{
	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	switch (g_Map[nTY][nTX])
	{
	case SIGN_GRASS:
		//WriteChar(nTX, nTY, "▓", FG_LIGHTGREEN);
		break;
	case SIGN_RIVER:
		//WriteChar(nTX, nTY, "▓", FG_LIGHTBLUE);
		break;
	default:
		UpdateMapPoint(SIGN_BULLET, nTX, nTY);
		WriteChar(nTX, nTY, "●", FG_LIGHTYELLOW);
		break;
	}
}

int CheckBulletCrash(int nBulletID)
{
	int bCrash = 0;

	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA) {
		switch (g_pBulletBox[nBulletID].eDrt)
		{
		case DRT_UP:
			if (g_Map[nTY - 1][nTX] == SIGN_TANK_E0 || g_Map[nTY - 1][nTX] == SIGN_TANK_E1) { //子弹打中敌军
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX && (g_pEnemies[i].nY + n) == nTY - 1) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									g_pBulletBox[nBulletID].bulValid = 1;
									g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
									goto END;
								}
							}
						}
					}
				}
			}
			else if (g_Map[nTY - 1][nTX] == SIGN_WALL0) { //子弹打中钢墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				bCrash = 1;
				goto END;
			}
			else if (g_Map[nTY - 1][nTX] == SIGN_WALL1) { //子弹打中水泥墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				g_Map[nTY - 1][nTX] = SIGN_EMPTY;
				WriteChar(nTX, nTY - 1, "  ", 0);
				bCrash = 1;
				goto END;
			}

			break;
		case DRT_DOWN:
			if (g_Map[nTY + 1][nTX] == SIGN_TANK_E0 || g_Map[nTY + 1][nTX] == SIGN_TANK_E1) {
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX && (g_pEnemies[i].nY + n) == nTY + 1) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									g_pBulletBox[nBulletID].bulValid = 1;
									g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
									goto END;
								}
							}
						}
					}
				}
			}
			else if (g_Map[nTY + 1][nTX] == SIGN_WALL0) { //子弹打中钢墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				bCrash = 1;
				goto END;
			}
			else if (g_Map[nTY + 1][nTX] == SIGN_WALL1) { //子弹打中水泥墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				g_Map[nTY + 1][nTX] = SIGN_EMPTY;
				WriteChar(nTX, nTY + 1, "  ", 0);
				bCrash = 1;
				goto END;
			}

			break;
		case DRT_LEFT:
			if (g_Map[nTY][nTX - 1] == SIGN_TANK_E0 || g_Map[nTY][nTX - 1] == SIGN_TANK_E1) {
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX - 1 && (g_pEnemies[i].nY + n) == nTY) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									g_pBulletBox[nBulletID].bulValid = 1;
									g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
									goto END;
								}
							}
						}
					}
				}
			}
			else if (g_Map[nTY][nTX - 1] == SIGN_WALL0) {
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				bCrash = 1;
				goto END;
			}
			else if (g_Map[nTY][nTX - 1] == SIGN_WALL1) { //子弹打中水泥墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				g_Map[nTY][nTX - 1] = SIGN_EMPTY;
				WriteChar(nTX-1, nTY, "  ", 0);
				bCrash = 1;
				goto END;
			}

			break;
		case DRT_RIGHT:
			if (g_Map[nTY][nTX + 1] == SIGN_TANK_E0 || g_Map[nTY][nTX + 1] == SIGN_TANK_E1) {
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX + 1 && (g_pEnemies[i].nY + n) == nTY) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									g_pBulletBox[nBulletID].bulValid = 1;
									g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
									goto END;
								}
							}
						}
					}
				}
			}
			else if (g_Map[nTY][nTX + 1] == SIGN_WALL0) {
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				bCrash = 1;
				goto END;
			}
			else if (g_Map[nTY][nTX + 1] == SIGN_WALL1) { //子弹打中水泥墙
				g_pBulletBox[nBulletID].bulValid = 1;
				g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
				g_Map[nTY][nTX + 1] = SIGN_EMPTY;
				WriteChar(nTX + 1, nTY, "  ", 0);
				bCrash = 1;
				goto END;
			}

			break;
		}
	}
	else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0) {

	}

END:
	return bCrash;
}

void MoveBullets(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (!g_pBulletBox[i].bulValid) {
			WipeBullet(i);

			if (CheckBulletCrash(i))
				continue;

			switch (g_pBulletBox[i].eDrt)
			{
			case DRT_UP:
				g_pBulletBox[i].nY -= 1;
				break;
			case DRT_DOWN:
				g_pBulletBox[i].nY += 1;
				break;
			case DRT_LEFT:
				g_pBulletBox[i].nX -= 1;
				break;
			case DRT_RIGHT:
				g_pBulletBox[i].nX += 1;
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

	int nTX = pTank->nX;
	int nTY = pTank->nY;

	int nBulletID = GetBullet();

	switch (pTank->eDrt)
	{
	case DRT_UP:
		if (g_Map[nTY - 1][nTX + 1] == SIGN_WALL0) //炮口前面一个点就是钢墙则不发射
			return;
		g_pBulletBox[nBulletID].nX = nTX + 1;
		g_pBulletBox[nBulletID].nY = nTY - 1;
		break;
	case DRT_DOWN:
		if (g_Map[nTY + 3][nTX + 1] == SIGN_WALL0)
			return;
		g_pBulletBox[nBulletID].nX = nTX + 1;
		g_pBulletBox[nBulletID].nY = nTY + 3;
		break;
	case DRT_LEFT:
		if (g_Map[nTY + 1][nTX - 1] == SIGN_WALL0)
			return;
		g_pBulletBox[nBulletID].nX = nTX - 1;
		g_pBulletBox[nBulletID].nY = nTY + 1;
		break;
	case DRT_RIGHT:
		if (g_Map[nTY + 1][nTX + 3] == SIGN_WALL0)
			return;
		g_pBulletBox[nBulletID].nX = nTX + 3;
		g_pBulletBox[nBulletID].nY = nTY + 1;
		break;
	}

	g_pBulletBox[nBulletID].eDrt = pTank->eDrt;
	g_pBulletBox[nBulletID].bulOwner = pTank->eType;
	g_pBulletBox[nBulletID].bulValid = 0;
	ShowBullet(nBulletID);
}

Bullet *InitBulletBox(void)
{
	Bullet *bulletBox = (Bullet *)calloc(BOX_CAPACITY, sizeof(Bullet));
	for (int i = 0; i < BOX_CAPACITY; i++) {
		bulletBox[i].nBulletID = i;
		bulletBox[i].bulValid = 1;
		bulletBox[i].bulOwner = SIGN_EMPTY;
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