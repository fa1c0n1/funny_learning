#ifndef _BULLET_H
#define _BULLET_H

#include "data.h"

extern int GetBullet(void);
extern void SetBulletPosition(int nBulletID, int nX, int nY);
extern void ResetBullet(int nBulletID);
extern void WipeBullet(int nBulletID);
extern void ShowBullet(int nBulletID);
extern void MoveBullets(void);
extern void FireBullet(Tank *pTank);
extern Bullet *InitBulletBox(void);
int DoWhenPlayerBeated(Tank *pPlayer, int nX, int nY);
int DoWhenEnemyBeated(int nX, int nY);

#endif // _BULLET_H