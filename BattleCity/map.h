#ifndef _MAP_H
#define _MAP_H

#include "comm.h"
#include "data.h"

extern void UpdateMapPoint(ObjType eType, uint nX, uint nY);
extern void InitDefaultMap(void);
extern void ShowMap(void);
extern void ClearAllBarrier(void);
extern void DestroyObject(Object **pObj);

#endif // _MAP_H
