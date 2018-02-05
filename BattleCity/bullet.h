#ifndef _BULLET_H
#define _BULLET_H

#include "data.h"

extern int GetBullet(void);
extern void WipeBullet(int nBulletID);
extern void ShowBullet(int nBulletID);
extern void MoveBullets(void);
extern void FireBullet(Tank *pTank);
extern Bullet *InitBulletBox(void);
extern void DestroyBulletBox(Bullet *pBulletBox);

#endif // _BULLET_H