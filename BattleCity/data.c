#include "data.h"

int g_Map[40][40] = { 
	{SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0}, //row=0
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=4
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=9
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=SIGN_WALL04
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=SIGN_WALL09
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=24
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=29
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, 
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0}, //row=34
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,SIGN_WALL1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,SIGN_WALL1,SIGN_WALL1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SIGN_WALL0},
	{SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0,SIGN_WALL0}, //row=39
};

int g_TankStatus[4][3][3] = {
		{ //UP
			{0,1,0},
			{1,1,1},
			{1,0,1}
		},
		{ //DOWN
			{1,0,1},
			{1,1,1},
			{0,1,0}
		},
		{ //LEFT
			{0,1,1},
			{1,1,0},
			{0,1,1}
		},
		{ //RIGHT
			{1,1,0},
			{0,1,1},
			{1,1,0}
		}
};


int g_PboxStatus[3][3] = {
		{ 0, 1, 0 }, { 1, 1, 1 }, { 0, 1, 0 }
};

#define DX      -32
#define DY      -5
Point g_text1[60] = {
		{ 0 + DX, 12 + DY },
		{ 0 + DX, 13 + DY },
		{ 0 + DX, 14 + DY },
		{ 0 + DX, 15 + DY },
		{ 0 + DX, 16 + DY },
		{ 1 + DX, 12 + DY },
		{ 1 + DX, 14 + DY },
		{ 1 + DX, 16 + DY },
		{ 2 + DX, 12 + DY },
		{ 2 + DX, 13 + DY },
		{ 2 + DX, 15 + DY },
		{ 2 + DX, 16 + DY }, //B

		{ 4 + DX, 16 + DY },
		{ 5 + DX, 15 + DY },
		{ 6 + DX, 14 + DY },
		{ 7 + DX, 13 + DY },
		{ 7 + DX, 14 + DY },
		{ 8 + DX, 12 + DY },
		{ 8 + DX, 14 + DY },
		{ 9 + DX, 13 + DY },
		{ 9 + DX, 14 + DY },
		{ 10 + DX, 14 + DY },
		{ 11 + DX, 15 + DY },
		{ 12 + DX, 16 + DY }, //A

		{ 14 + DX, 12 + DY },
		{ 15 + DX, 12 + DY },
		{ 16 + DX, 12 + DY },
		{ 16 + DX, 13 + DY },
		{ 16 + DX, 14 + DY },
		{ 16 + DX, 15 + DY },
		{ 16 + DX, 16 + DY },
		{ 17 + DX, 12 + DY },
		{ 18 + DX, 12 + DY }, //T

		{ 20 + DX, 12 + DY },
		{ 21 + DX, 12 + DY },
		{ 22 + DX, 12 + DY },
		{ 22 + DX, 13 + DY },
		{ 22 + DX, 14 + DY },
		{ 22 + DX, 15 + DY },
		{ 22 + DX, 16 + DY },
		{ 23 + DX, 12 + DY },
		{ 24 + DX, 12 + DY }, //T

		{ 26 + DX, 12 + DY },
		{ 26 + DX, 13 + DY },
		{ 26 + DX, 14 + DY },
		{ 26 + DX, 15 + DY },
		{ 26 + DX, 16 + DY },
		{ 27 + DX, 16 + DY },
		{ 28 + DX, 16 + DY }, //L

		{ 30 + DX, 12 + DY },
		{ 30 + DX, 13 + DY },
		{ 30 + DX, 14 + DY },
		{ 30 + DX, 15 + DY },
		{ 30 + DX, 16 + DY },
		{ 31 + DX, 12 + DY },
		{ 31 + DX, 14 + DY },
		{ 31 + DX, 16 + DY },
		{ 32 + DX, 12 + DY },
		{ 32 + DX, 14 + DY },
		{ 32 + DX, 16 + DY }, //E
};
#undef DX
#undef DY

#define DX    -20
#define DY    -3
Point g_text2[32] = {
		{ 0 + DX, 18 + DY },
		{ 0 + DX, 19 + DY },
		{ 0 + DX, 20 + DY },
		{ 1 + DX, 17 + DY },
		{ 1 + DX, 21 + DY },
		{ 2 + DX, 17 + DY },
		{ 2 + DX, 21 + DY }, //C

		{ 5 + DX, 17 + DY },
		{ 5 + DX, 21 + DY },
		{ 6 + DX, 17 + DY },
		{ 6 + DX, 18 + DY },
		{ 6 + DX, 19 + DY },
		{ 6 + DX, 20 + DY },
		{ 6 + DX, 21 + DY },
		{ 7 + DX, 17 + DY },
		{ 7 + DX, 21 + DY }, //I

		{ 9 + DX, 17 + DY },
		{ 10 + DX, 17 + DY },
		{ 11 + DX, 17 + DY },
		{ 11 + DX, 18 + DY },
		{ 11 + DX, 19 + DY },
		{ 11 + DX, 20 + DY },
		{ 11 + DX, 21 + DY },
		{ 12 + DX, 17 + DY },
		{ 13 + DX, 17 + DY }, //T

		{ 15 + DX, 17 + DY },
		{ 16 + DX, 18 + DY },
		{ 17 + DX, 19 + DY },
		{ 17 + DX, 20 + DY },
		{ 17 + DX, 21 + DY },
		{ 18 + DX, 18 + DY },
		{ 19 + DX, 17 + DY }, //Y
};
#undef DX
#undef DY


Bullet *g_pBulletBox = NULL;
Tank *g_pEnemies = NULL;
Tank *g_pTankA = NULL;
Tank *g_pTankB = NULL;
Pillbox *g_pPillbox = NULL;
ObjType g_eNodeType = SIGN_WALL1;
int g_nEnNum = ENEMY_NMAX;

clock_t g_startTime = 0;
