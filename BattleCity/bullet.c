#include "bullet.h"
#include "draw_tool.h"
#include "map.h"

#include <stdlib.h>

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
	WriteChar(g_BulletBox[nBulletID].nX, g_BulletBox[nBulletID].nY, "¡ñ", FG_LIGHTYELLOW);
}

void MoveBullets(void)
{
	for (int i = 0; i < BOX_CAPACITY; i++) {
		if (!g_BulletBox[i].bulValid) {
			WipeBullet(i);
			switch (g_BulletBox[i].eDrt)
			{
			case DRT_UP:
				if (g_BulletBox[i].nY - 1 <= 0) {
					g_BulletBox[i].bulValid = 1;
					g_BulletBox[i].bulOwner = SIGN_EMPTY;
					continue;
				}
				g_BulletBox[i].nY -= 1;
				break;
			case DRT_DOWN:
				if (g_BulletBox[i].nY + 1 >= 39) {
					g_BulletBox[i].bulValid = 1;
					g_BulletBox[i].bulOwner = SIGN_EMPTY;
					continue;
				}
				g_BulletBox[i].nY += 1;
				break;
			case DRT_LEFT:
				if (g_BulletBox[i].nX - 1 <= 0) {
					g_BulletBox[i].bulValid = 1;
					g_BulletBox[i].bulOwner = SIGN_EMPTY;
					continue;
				}
				g_BulletBox[i].nX -= 1;
				break;
			case DRT_RIGHT:
				if (g_BulletBox[i].nX + 1 >= 39) {
					g_BulletBox[i].bulValid = 1;
					g_BulletBox[i].bulOwner = SIGN_EMPTY;
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
	g_BulletBox[nBulletID].bulOwner = pTank->eType;

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