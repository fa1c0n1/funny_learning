#include "enemy.h"

#include <stdlib.h>
#include <time.h>

Tank *InitEnemies(void)
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

void RandomMoveEnemies(void)
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
			if ((endTime - g_startTime) % 150 + 150 > 295) {
				FireBullet(&g_pEnemies[i]);
			}
		}
	}
}