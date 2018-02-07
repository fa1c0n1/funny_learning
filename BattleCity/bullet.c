#include "bullet.h"
#include "draw_tool.h"
#include "map.h"
#include "player.h"
#include "pillbox.h"

#include <stdlib.h>

//从子弹仓库获取子弹
int GetBullet(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (g_pBulletBox[i].bulValid)
			return g_pBulletBox[i].nBulletID;
	}
}

//擦除子弹
void WipeBullet(int nBulletID)
{
	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	switch (g_Map[nTY][nTX])
	{
	case SIGN_GRASS:
	case SIGN_RIVER:
	case SIGN_TANK_PA:
	case SIGN_TANK_PB:
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		break;
	default:
		UpdateMapPoint(SIGN_EMPTY, nTX, nTY);
		WriteChar(nTX, nTY, "  ", 0);
		break;
	}
}

//显示子弹
void ShowBullet(int nBulletID)
{
	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;
	WORD wAttr;

	if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB)
		wAttr = FG_LIGHTYELLOW;
	else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1)
		wAttr = FG_GRAY;

	switch (g_Map[nTY][nTX])
	{
	case SIGN_GRASS:
	case SIGN_RIVER:
	case SIGN_TANK_PA:
	case SIGN_TANK_PB:
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		break;
	default:
		UpdateMapPoint(SIGN_BULLET, nTX, nTY);
		WriteChar(nTX, nTY, "●", wAttr);
		break;
	}
}

//检测子弹所有的碰撞
int CheckBulletCrash(int nBulletID)
{
	int bCrash = 0;

	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	//判断子弹之间的碰撞
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (i == nBulletID)
			continue;
		if (!g_pBulletBox[i].bulValid) {
			if (g_pBulletBox[nBulletID].nX == g_pBulletBox[i].nX && g_pBulletBox[nBulletID].nY == g_pBulletBox[i].nY) {
				switch (g_pBulletBox[nBulletID].bulOwner) //根据子弹属主分别处理
				{
				case SIGN_TANK_PA:
				case SIGN_TANK_PB:
					if (g_pBulletBox[i].bulOwner == SIGN_TANK_E0 || g_pBulletBox[i].bulOwner == SIGN_TANK_E1) {
						WipeBullet(i);
						ResetBullet(i);
						bCrash = 1;
						goto END;
					}
					break;
				case SIGN_TANK_E0:
				case SIGN_TANK_E1:
					if (g_pBulletBox[i].bulOwner == SIGN_TANK_PA || g_pBulletBox[i].bulOwner == SIGN_TANK_PB) {
						WipeBullet(i);
						ResetBullet(i);
						bCrash = 1;
						goto END;
					}
					break;
				}
			}
		}
	}

	switch (g_pBulletBox[nBulletID].eDrt)
	{
	case DRT_UP:  //子弹向上走时，不需要判断与碉堡的碰撞，因为碉堡就在最下方
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //玩家的子弹
			if (g_Map[nTY - 1][nTX] == SIGN_TANK_E0 || g_Map[nTY - 1][nTX] == SIGN_TANK_E1) { //子弹打中敌军
				if (DoWhenEnemyBeated(nTX, nTY - 1)) {
					bCrash = 1;
					goto END;
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //敌军的子弹
			if (g_Map[nTY - 1][nTX] == SIGN_TANK_PA) { //子弹打中玩家A
				if (DoWhenPlayerBeated(g_pTankA, nTX, nTY - 1)) {
					bCrash = 1;
					goto END;
				}
			}
			else if (g_Map[nTY - 1][nTX] == SIGN_TANK_PB) { //子弹打中玩家B
				// TODO:
			}
		}

		if (DoWhenBarrierBeated(nTX, nTY - 1)) {
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_DOWN:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //玩家的子弹
			if (g_Map[nTY + 1][nTX] == SIGN_TANK_E0 || g_Map[nTY + 1][nTX] == SIGN_TANK_E1) { //子弹打中敌军
				if (DoWhenEnemyBeated(nTX, nTY + 1)) {
					bCrash = 1;
					goto END;
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //敌军的子弹
			if (g_Map[nTY + 1][nTX] == SIGN_TANK_PA) { //子弹打中玩家A
				if (DoWhenPlayerBeated(g_pTankA, nTX, nTY + 1)) {
					bCrash = 1;
					goto END;
				}
			}
			else if (g_Map[nTY + 1][nTX] == SIGN_TANK_PB) { //子弹打中玩家B
				// TODO:
			}
		}
		
		if (DoWhenBarrierBeated(nTX, nTY + 1)) {
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_LEFT:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //玩家的子弹
			if (g_Map[nTY][nTX - 1] == SIGN_TANK_E0 || g_Map[nTY][nTX - 1] == SIGN_TANK_E1) { //子弹打中敌军
				if (DoWhenEnemyBeated(nTX - 1, nTY)) {
					bCrash = 1;
					goto END;
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //敌军的子弹
			if (g_Map[nTY][nTX - 1] == SIGN_TANK_PA) { //子弹打中玩家A
				if (DoWhenPlayerBeated(g_pTankA, nTX - 1, nTY)) {
					bCrash = 1;
					goto END;
				}
			}
			else if (g_Map[nTY][nTX - 1] == SIGN_TANK_PB) {
				//TODO:
			}
		}
		
		if (DoWhenBarrierBeated(nTX - 1, nTY)) {
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_RIGHT:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //玩家的子弹
			if (g_Map[nTY][nTX + 1] == SIGN_TANK_E0 || g_Map[nTY][nTX + 1] == SIGN_TANK_E1) { //子弹打中敌军
				if (DoWhenEnemyBeated(nTX + 1, nTY)) {
					bCrash = 1;
					goto END;
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //敌军的子弹
			if (g_Map[nTY][nTX + 1] == SIGN_TANK_PA) { //子弹打中玩家A
				if (DoWhenPlayerBeated(g_pTankA, nTX + 1, nTY)) {
					bCrash = 1;
					goto END;
				}
			}
			else if (g_Map[nTY][nTX + 1] == SIGN_TANK_PB) {
				//TODO:
			}
		}

		if (DoWhenBarrierBeated(nTX + 1, nTY)) {
			bCrash = 1;
			goto END;
		}

		break;
	}

END:
	return bCrash;
}

//当玩家被敌军子弹打中时的处理
int DoWhenPlayerBeated(Tank *pPlayer, int nX, int nY)
{
	int bRet = 0;

	if (pPlayer == NULL)
		return bRet;

	if (!pPlayer->bDead) {
		for (int m = 0; m < 3; m++) {
			for (int n = 0; n < 3; n++) {
				if ((pPlayer->nX + m) == nX && (pPlayer->nY + n) == nY) {
					pPlayer->bDead = 1;
					WipeTank(pPlayer);
					bRet = 1;
					goto END;
				}
			}
		}
	}

END:
	return bRet;
}

//当敌军被玩家子弹打中时的处理
int DoWhenEnemyBeated(int nX, int nY)
{
	int bRet = 0;

	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!g_pEnemies[i].bDead) {
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if ((g_pEnemies[i].nX + m) == nX && (g_pEnemies[i].nY + n) == nY) {
						g_pEnemies[i].bDead = 1;
						WipeTank(&g_pEnemies[i]);
						g_nEnNum--;
						bRet = 1;
						goto END;
					}
				}
			}
		}
	}

END:
	return bRet;
}

//子弹打中障碍物时
int DoWhenBarrierBeated(int nX, int nY)
{
	int bRet = 0;

	if (g_Map[nY][nX] == SIGN_WALL0) { //子弹打中钢墙
		bRet = 1;
	}
	else if (g_Map[nY][nX] == SIGN_WALL1) { //子弹打中水泥墙
		g_Map[nY][nX] = SIGN_EMPTY;
		WriteChar(nX, nY, "  ", 0);
		bRet = 1;
	}
	else if (g_Map[nY][nX] == SIGN_PILLBOX) { //子弹打中碉堡
		g_pPillbox->bDead = 1;
		ShowPillbox(g_pPillbox);
		bRet = 1;
	}

	return bRet;
}

//移动所有已发射的子弹
void MoveBullets(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (!g_pBulletBox[i].bulValid) {
			WipeBullet(i);

			if (CheckBulletCrash(i)) {
				ResetBullet(i);
				continue;
			}

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

//重置子弹
void ResetBullet(int nBulletID)
{
	g_pBulletBox[nBulletID].bulValid = 1;
	g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
}

//设置子弹在地图上的位置
void SetBulletPosition(int nBulletID, int nX, int nY)
{
	g_pBulletBox[nBulletID].nX = nX;
	g_pBulletBox[nBulletID].nY = nY;
}

//发射子弹
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
		SetBulletPosition(nBulletID, nTX + 1, nTY - 1);
		break;
	case DRT_DOWN:
		if (g_Map[nTY + 3][nTX + 1] == SIGN_WALL0)
			return;
		SetBulletPosition(nBulletID, nTX + 1, nTY + 3);
		break;
	case DRT_LEFT:
		if (g_Map[nTY + 1][nTX - 1] == SIGN_WALL0)
			return;
		SetBulletPosition(nBulletID, nTX - 1, nTY + 1);
		break;
	case DRT_RIGHT:
		if (g_Map[nTY + 1][nTX + 3] == SIGN_WALL0)
			return;
		SetBulletPosition(nBulletID, nTX + 3, nTY + 1);
		break;
	}

	g_pBulletBox[nBulletID].eDrt = pTank->eDrt;
	g_pBulletBox[nBulletID].bulOwner = pTank->eType;
	g_pBulletBox[nBulletID].bulValid = 0;
	ShowBullet(nBulletID);
}

//初始化子弹仓库
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
