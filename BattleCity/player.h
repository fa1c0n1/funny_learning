#ifndef _PLAYER_H
#define _PLAYER_H

#include "data.h"

extern Tank *CreatePlayer(TankType eType, uint nX, uint nY, Direction eDrt);
extern void ShowTank(Tank *pTank);
extern void WipeTank(Tank *pTank);
extern void MoveTank(Tank *pTank, Direction eDrt);

#endif // _PLAYER_H