#include "player.h"
#include "map.h"
#include "draw_tool.h"
#include "windows.h"

#include <stdlib.h>

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
				WriteChar(pTank->nX + j, pTank->nY + i, "¡ö", wAttr);
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

void DestroyTank(Tank *pTank)
{
	if (pTank != NULL) {
		WipeTank(pTank);
		free(pTank);
		pTank = NULL;
	}
}