#include "map.h"

void UpdateMapPoint(ObjType eType, uint nX, uint nY)
{
	g_Map[nY][nX] = eType;
}

//清除地图上所有的障碍物
void ClearAllBarrier(void)
{
	//恢复碉堡周围的水泥墙
	for (int i = 34; i <= 35; i++) {
		for (int j = 15; j <= 21; j++) {
			UpdateMapPoint(SIGN_WALL1, j, i);
		}
	}

	for (int i = 36; i <= 38; i++) {
		for (int j = 15; j <= 21; j++) {
			if (j < 17 || j > 19) {
				UpdateMapPoint(SIGN_WALL1, j, i);
			}
		}
	}

	for (int i = 1; i < 39; i++) {
		for (int j = 1; j < 39; j++) {
			if (j >= 15 && j <= 21) {
				if (i >= 34 && i <= 38)
					continue;
			}

			if (g_Map[i][j] != SIGN_EMPTY) {
				g_Map[i][j] = SIGN_EMPTY;
			}
		}
	}
}

//初始化默认地图
void InitDefaultMap(void)
{
	//布置水泥墙
	for (int i = 7; i <= 15; i++) {
		for (int j = 7; j <= 9; j++) {
			g_Map[i][j] = SIGN_WALL1;
		}
	}

	//布置钢墙
	for (int i = 21; i <= 22; i++) {
		for (int j = 5; j <= 7; j++) {
			g_Map[i][j] = SIGN_WALL0;
		}
	}
	
	//布置河流
	for (int i = 21; i <= 22; i++) {
		for (int j = 16; j <= 17; j++) {
			g_Map[i][j] = SIGN_RIVER;
		}
	}

	for (int i = 16; i <= 20; i++) {
		for (int j = 23; j <= 24; j++) {
			g_Map[i][j] = SIGN_RIVER;
		}
	}

	//布置草地
	for (int i = 26; i <= 28; i++) {
		for (int j = 19; j <= 25; j++) {
			g_Map[i][j] = SIGN_GRASS;
		}
	}
}

void ShowMap(void)
{
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			switch (g_Map[i][j])
			{
			case SIGN_WALL0:
				WriteChar(j, i, "■", FG_HIGHWHITE);
				break;
			case SIGN_WALL1:
				WriteChar(j, i, "■", FG_YELLOW);
				break;
			case SIGN_GRASS:
				WriteChar(j, i, "▓", FG_GREEN);
				break;
			case SIGN_RIVER:
				WriteChar(j, i, "▓", FG_LIGHTBLUE);
				break;
			default:
				WriteChar(j, i, "  ", 0);
				break;
			}
		}
	}
}

//释放游戏中分配的动态内存
void DestroyObject(Object **pObj)
{
	if (pObj == NULL)
		return;

	if (*pObj != NULL) {
		free(*pObj);
		*pObj = NULL;
	}
}

