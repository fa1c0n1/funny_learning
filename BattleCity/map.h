#ifndef _MAP_H
#define _MAP_H

#include "comm.h"
#include "data.h"

extern void UpdateMapPoint(ObjType eType, uint nX, uint nY);
extern void InitDefaultMap(void);
extern void ShowMap(void);
extern void ClearAllBarrier(void);
extern void DestroyObject(Object **pObj);
extern int DrawMapResult(void);
extern void DrawMapMouseEventProc(MOUSE_EVENT_RECORD mer);
extern int DrawMapKeyEventProc(KEY_EVENT_RECORD ker);

#endif // _MAP_H
