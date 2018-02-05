#include "map.h"

void UpdateMapPoint(ObjType eType, uint nX, uint nY)
{
	g_Map[nY][nX] = eType;
}