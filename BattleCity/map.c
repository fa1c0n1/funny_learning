#include "map.h"

void UpdateMapPoint(ObjType eType, uint nX, uint nY)
{
	g_Map[nY][nX] = eType;
}

void ShowMap(void)
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			switch (g_Map[i][j])
			{
			case SIGN_WALL0:
				WriteChar(j, i, "¡ö", FG_HIGHWHITE);
				break;
			case SIGN_WALL1:
				WriteChar(j, i, "¡ö", FG_YELLOW);
				break;
			case SIGN_GRASS:
				WriteChar(j, i, "¨ˆ", FG_GREEN);
				break;
			case SIGN_RIVER:
				WriteChar(j, i, "¨ˆ", FG_LIGHTBLUE);
				break;
			default:
				break;
			}
		}
	}
}


