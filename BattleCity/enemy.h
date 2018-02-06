#ifndef _ENEMY_H
#define _ENEMY_H

#include "data.h"

extern Tank *InitEnemies(void);
extern void ShowEnemies(Tank *pEnemies);
extern void RandomMoveEnemies(void);

#endif  // _ENEMY_H