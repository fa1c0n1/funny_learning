#include "bullet.h"
#include "draw_tool.h"
#include "map.h"
#include "player.h"

#include <stdlib.h>

//���ӵ��ֿ��ȡ�ӵ�
int GetBullet(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (g_pBulletBox[i].bulValid)
			return g_pBulletBox[i].nBulletID;
	}
}

//�����ӵ�
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

//��ʾ�ӵ�
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
		WriteChar(nTX, nTY, "��", wAttr);
		break;
	}
}

//����ӵ����е���ײ
int CheckBulletCrash(int nBulletID)
{
	int bCrash = 0;

	int nTX = g_pBulletBox[nBulletID].nX;
	int nTY = g_pBulletBox[nBulletID].nY;

	//�ж��ӵ�֮�����ײ
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (i == nBulletID)
			continue;
		if (!g_pBulletBox[i].bulValid) {
			if (g_pBulletBox[nBulletID].nX == g_pBulletBox[i].nX && g_pBulletBox[nBulletID].nY == g_pBulletBox[i].nY) {
				switch (g_pBulletBox[nBulletID].bulOwner)
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
	case DRT_UP:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //��ҵ��ӵ�
			if (g_Map[nTY - 1][nTX] == SIGN_TANK_E0 || g_Map[nTY - 1][nTX] == SIGN_TANK_E1) { //�ӵ����ео�
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX && (g_pEnemies[i].nY + n) == nTY - 1) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									goto END;
								}
							}
						}
					}
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //�о����ӵ�
			if (g_Map[nTY - 1][nTX] == SIGN_TANK_PA) { //�ӵ��������A
				if (!g_pTankA->bDead) {
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							if ((g_pTankA->nX + m) == nTX && (g_pTankA->nY + n) == nTY - 1) {
								g_pTankA->bDead = 1;
								WipeTank(g_pTankA);
								bCrash = 1;
								goto END;
							}
						}
					}
				}
			}
			else if (g_Map[nTY - 1][nTX] == SIGN_TANK_PB) { //�ӵ��������B
				// TODO:
			}
		}

		if (g_Map[nTY - 1][nTX] == SIGN_WALL0) { //�ӵ����и�ǽ
			bCrash = 1;
			goto END;
		}
		else if (g_Map[nTY - 1][nTX] == SIGN_WALL1) { //�ӵ�����ˮ��ǽ
			g_Map[nTY - 1][nTX] = SIGN_EMPTY;
			WriteChar(nTX, nTY - 1, "  ", 0);
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_DOWN:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //��ҵ��ӵ�
			if (g_Map[nTY + 1][nTX] == SIGN_TANK_E0 || g_Map[nTY + 1][nTX] == SIGN_TANK_E1) { //�ӵ����ео�
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX && (g_pEnemies[i].nY + n) == nTY + 1) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									goto END;
								}
							}
						}
					}
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //�о����ӵ�
			if (g_Map[nTY + 1][nTX] == SIGN_TANK_PA) { //�ӵ��������A
				if (!g_pTankA->bDead) {
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							if ((g_pTankA->nX + m) == nTX && (g_pTankA->nY + n) == nTY + 1) {
								g_pTankA->bDead = 1;
								WipeTank(g_pTankA);
								bCrash = 1;
								goto END;
							}
						}
					}
				}
			}
			else if (g_Map[nTY + 1][nTX] == SIGN_TANK_PB) { //�ӵ��������B
				// TODO:
			}
		}
		
		if (g_Map[nTY + 1][nTX] == SIGN_WALL0) {
			bCrash = 1;
			goto END;
		}
		else if (g_Map[nTY + 1][nTX] == SIGN_WALL1) {
			g_Map[nTY + 1][nTX] = SIGN_EMPTY;
			WriteChar(nTX, nTY + 1, "  ", 0);
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_LEFT:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //��ҵ��ӵ�
			if (g_Map[nTY][nTX - 1] == SIGN_TANK_E0 || g_Map[nTY][nTX - 1] == SIGN_TANK_E1) { //�ӵ����ео�
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX - 1 && (g_pEnemies[i].nY + n) == nTY) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									goto END;
								}
							}
						}
					}
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //�о����ӵ�
			if (g_Map[nTY][nTX - 1] == SIGN_TANK_PA) { //�ӵ��������A
				if (!g_pTankA->bDead) {
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							if ((g_pTankA->nX + m) == nTX - 1 && (g_pTankA->nY + n) == nTY) {
								g_pTankA->bDead = 1;
								WipeTank(g_pTankA);
								bCrash = 1;
								goto END;
							}
						}
					}
				}
			}
			else if (g_Map[nTY][nTX - 1] == SIGN_TANK_PB) {
				//TODO:
			}
		}
		
		if (g_Map[nTY][nTX - 1] == SIGN_WALL0) {
			bCrash = 1;
			goto END;
		}
		else if (g_Map[nTY][nTX - 1] == SIGN_WALL1) {
			g_Map[nTY][nTX - 1] = SIGN_EMPTY;
			WriteChar(nTX - 1, nTY, "  ", 0);
			bCrash = 1;
			goto END;
		}

		break;
	case DRT_RIGHT:
		if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PA || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_PB) { //��ҵ��ӵ�
			if (g_Map[nTY][nTX + 1] == SIGN_TANK_E0 || g_Map[nTY][nTX + 1] == SIGN_TANK_E1) { //�ӵ����ео�
				for (int i = 0; i < ENEMY_NMAX; i++) {
					if (!g_pEnemies[i].bDead) {
						for (int m = 0; m < 3; m++) {
							for (int n = 0; n < 3; n++) {
								if ((g_pEnemies[i].nX + m) == nTX + 1 && (g_pEnemies[i].nY + n) == nTY) {
									g_pEnemies[i].bDead = 1;
									WipeTank(&g_pEnemies[i]);
									bCrash = 1;
									goto END;
								}
							}
						}
					}
				}
			}
		}
		else if (g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E0 || g_pBulletBox[nBulletID].bulOwner == SIGN_TANK_E1) { //�о����ӵ�
			if (g_Map[nTY][nTX + 1] == SIGN_TANK_PA) { //�ӵ��������A
				if (!g_pTankA->bDead) {
					for (int m = 0; m < 3; m++) {
						for (int n = 0; n < 3; n++) {
							if ((g_pTankA->nX + m) == nTX + 1 && (g_pTankA->nY + n) == nTY) {
								g_pTankA->bDead = 1;
								WipeTank(g_pTankA);
								bCrash = 1;
								goto END;
							}
						}
					}
				}
			}
			else if (g_Map[nTY][nTX + 1] == SIGN_TANK_PB) {
				//TODO:
			}
		}

		if (g_Map[nTY][nTX + 1] == SIGN_WALL0) {
			bCrash = 1;
			goto END;
		}
		else if (g_Map[nTY][nTX + 1] == SIGN_WALL1) {
			g_Map[nTY][nTX + 1] = SIGN_EMPTY;
			WriteChar(nTX + 1, nTY, "  ", 0);
			bCrash = 1;
			goto END;
		}

		break;
	}

END:
	return bCrash;
}

//�ƶ������ѷ�����ӵ�
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

//�����ӵ�
void ResetBullet(int nBulletID)
{
	g_pBulletBox[nBulletID].bulValid = 1;
	g_pBulletBox[nBulletID].bulOwner = SIGN_EMPTY;
}

//�����ӵ��ڵ�ͼ�ϵ�λ��
void SetBulletPosition(int nBulletID, int nX, int nY)
{
	g_pBulletBox[nBulletID].nX = nX;
	g_pBulletBox[nBulletID].nY = nY;
}

//�����ӵ�
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
		if (g_Map[nTY - 1][nTX + 1] == SIGN_WALL0) //�ڿ�ǰ��һ������Ǹ�ǽ�򲻷���
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

//��ʼ���ӵ��ֿ�
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
