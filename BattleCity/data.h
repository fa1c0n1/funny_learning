#ifndef _DATA_H
#define _DATA_H

#include "comm.h"

#include <time.h>

//地图上每个点的类型
typedef enum {
	SIGN_EMPTY,
	SIGN_WALL0,       //钢墙(不可击毁)
	SIGN_WALL1,       //水泥墙(可击毁)
	SIGN_TANK_PA,     //玩家坦克A
	SIGN_TANK_PB,     //玩家坦克B 
	SIGN_TANK_E0,     //敌军坦克0
	SIGN_TANK_E1,     //敌军坦克1
	SIGN_BULLET,      //子弹
	SIGN_GRASS,       //草地
	SIGN_RIVER        //河流
} ObjType;

//方向
typedef enum {
	DRT_UP,
	DRT_DOWN,
	DRT_LEFT,
	DRT_RIGHT,
	DRT_NONE
} Direction;

typedef ObjType TankType;

typedef struct {
	int nX;
	int nY;
} Point;

//地图上的对象
typedef struct {
	uint bulValid : 1;     //子弹是否有效
	uint bDead : 1;        //坦克是否死亡
	Direction eDrt : 4;    //方向
	ObjType eType : 8;     //钢墙 土墙 草地 河流 沙地等, 坦克A 坦克B 敌军坦克等
	uint nX : 8;          //对象左上角的横坐标
	uint nY : 8;          //对象左上角的纵坐标
	ObjType bulOwner : 8; //子弹属主
	uint nBulletID : 16;  //子弹ID
	uint nEnID : 16;  //敌军ID
} Object;

typedef Object Tank;
typedef Object Bullet;

typedef struct {
	uint nKey;         //对应的按键
	int(*func)(void); //对应的函数
	char szText[64];   //对应的文字说明
} MenuItem;


//前景色
#define FG_BLUE             0x0001
#define FG_GREEN            0x0002
#define FG_TURQUOISE        0x0003
#define FG_RED              0x0004
#define FG_PURPLE           0x0005
#define FG_YELLOW           0x0006
#define FG_WHITE            0x0007
#define FG_GRAY             0x0008
#define FG_LIGHTBLUE        0x0009
#define FG_LIGHTGREEN       0x000A
#define FG_LIGHTTURQUOISE   0x000B
#define FG_LIGHTRED         0x000C
#define FG_LIGHTPURPLE      0x000D
#define FG_LIGHTYELLOW      0x000E
#define FG_HIGHWHITE        0x000F

//背景色
#define BG_BLUE             0x0010
#define BG_GREEN            0x0020
#define BG_TURQUOISE        0x0030
#define BG_RED              0x0040
#define BG_PURPLE           0x0050
#define BG_YELLOW           0x0060
#define BG_WHITE            0x0070
#define BG_GRAY             0x0080
#define BG_LIGHTBLUE        0x0090
#define BG_LIGHTGREEN       0x00A0
#define BG_LIGHTTURQUOISE   0x00B0
#define BG_LIGHTRED         0x00C0
#define BG_LIGHTPURPLE      0x00D0
#define BG_LIGHTYELLOW      0x00E0
#define BG_HIGHWHITE        0x00F0

//按键值
#define KEY_0               48
#define KEY_1               49
#define KEY_2               50
#define KEY_3               51
#define KEY_4               52
#define KEY_5               53
#define KEY_6               54
#define KEY_ENTER           13
#define KEY_EXIT            27
#define KEY_UP              72
#define KEY_DOWN            80
#define KEY_LEFT            75
#define KEY_RIGHT           77
#define KEY_DIRECTION       224

//BulletBox的容量
#define BOX_CAPACITY        100

//敌军的数量最大值
#define ENEMY_NMAX           5

extern int g_Map[40][40];          //存放地图的各个点
extern int g_TankStatus[4][3][3];  //坦克上下左右四个方向的状态
extern Bullet *g_pBulletBox;       //子弹仓库
extern Tank *g_pTankA;             //玩家A
extern Tank *g_pTankB;             //玩家B
extern Tank *g_pEnemies;           //敌军
extern int g_nEnNum;               //敌军数量
extern ObjType g_eNodeType;        //表示地图上点的类型，编辑地图时用于切换类型

extern clock_t g_startTime;        

#endif  // _DATA_H