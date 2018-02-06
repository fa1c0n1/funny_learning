#include "player.h"
#include "map.h"
#include "draw_tool.h"
#include "windows.h"

#include <stdlib.h>

Tank *CreatePlayer(TankType eType, uint nX, uint nY, Direction eDrt)
{
	if (eType != SIGN_TANK_PA && eType != SIGN_TANK_PB)
		return NULL;

	Tank *pTank = (Tank *)calloc(1, sizeof(Tank));
	pTank->eType = eType;
	pTank->nX = nX;
	pTank->nY = nY;
	pTank->eDrt = eDrt;
	pTank->bDead = 0;

	return pTank;
}

void WipeTank(Tank *pTank)
{
	if (pTank == NULL)
		return;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (g_Map[pTank->nY + i][pTank->nX + j] != SIGN_GRASS) {
				UpdateMapPoint(SIGN_EMPTY, pTank->nX + j, pTank->nY + i);
				WriteChar(pTank->nX + j, pTank->nY + i, "  ", 0);
			}
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
	case SIGN_TANK_PB:
		wAttr = FG_LIGHTGREEN;
		break;
	case SIGN_TANK_E0:
		wAttr = FG_LIGHTRED;
		break;
	default:
		break;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (g_Map[pTank->nY + i][pTank->nX+j] != SIGN_GRASS)
				UpdateMapPoint(pTank->eType, pTank->nX + j, pTank->nY + i);
			if (g_TankStatus[pTank->eDrt][i][j] == 1) {
				if (g_Map[pTank->nY + i][pTank->nX + j] != SIGN_GRASS)
					WriteChar(pTank->nX + j, pTank->nY + i, "¡ö", wAttr);
				else
					WriteChar(pTank->nX + j, pTank->nY + i, "¨ˆ", FG_GREEN);
			}
		}
	}
}

int CheckTankCrash(Tank *pTank)
{
	int bCrash = 0;

	if (pTank == NULL)
		goto END;

	int nTX = pTank->nX;
	int nTY = pTank->nY;

	switch (pTank->eDrt)
	{
	case DRT_UP:
		for (int i = 0; i < 3; i++) {
			if (g_Map[nTY - 1][nTX + i] != SIGN_EMPTY && g_Map[nTY - 1][nTX + i] != SIGN_GRASS) {
				bCrash = 1;
				goto END;
			}
		}
		break;
	case DRT_DOWN:
		for (int i = 0; i < 3; i++) {
			if (g_Map[nTY + 3][nTX + i] != SIGN_EMPTY && g_Map[nTY + 3][nTX + i] != SIGN_GRASS) {
				bCrash = 1;
				goto END;
			}
		}
		break;
	case DRT_LEFT:
		for (int i = 0; i < 3; i++) {
			if (g_Map[nTY + i][nTX - 1] != SIGN_EMPTY && g_Map[nTY + i][nTX - 1] != SIGN_GRASS) {
				bCrash = 1;
				goto END;
			}
		}
		break;
	case DRT_RIGHT:
		for (int i = 0; i < 3; i++) {
			if (g_Map[nTY + i][nTX + 3] != SIGN_EMPTY && g_Map[nTY + i][nTX + 3] != SIGN_GRASS) {
				bCrash = 1;
				goto END;
			}
		}
		break;
	}

END:
	return bCrash;
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

	if (CheckTankCrash(pTank))
		return;

	WipeTank(pTank);
	switch (pTank->eDrt)
	{
	case DRT_UP:
		pTank->nY -= 1;
		break;
	case DRT_DOWN:
		pTank->nY += 1;
		break;
	case DRT_LEFT:
		pTank->nX -= 1;
		break;
	case DRT_RIGHT:
		pTank->nX += 1;
		break;
	}

	ShowTank(pTank);
}
