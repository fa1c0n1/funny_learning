#pragma once

// 判断指定的按键是否按下
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// 判断指定的按键是否弹起
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//地图上每个点的类型
typedef enum {
	SIGN_EMPTY,
	SIGN_WALL0,       //钢墙(不可击毁)
	SIGN_WALL1,       //水泥墙(可击毁)
	SIGN_GRASS,       //草地
	SIGN_RIVER,       //河流
	SIGN_PILLBOX,     //玩家碉堡
	SIGN_PILLBOX_HIT,  //玩家碉堡(被击中)
	SIGN_TANK_PA,     //玩家坦克A
	SIGN_TANK_PB,     //玩家坦克B 
	SIGN_TANK_E0,     //敌军坦克0
	SIGN_TANK_E1,     //敌军坦克1
	SIGN_BULLET0,     //我方子弹
	SIGN_BULLET1,     //敌军子弹
} ObjType;

#define DRT_UP        0
#define DRT_DOWN      1
#define DRT_LEFT      2
#define DRT_RIGHT     3
#define DRT_NONE      4

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
#define KEY_8               56
#define KEY_ENTER           13
#define KEY_EXIT            27
#define KEY_UP              72
#define KEY_DOWN            80
#define KEY_LEFT            75
#define KEY_RIGHT           77
#define KEY_DIRECTION       224

//同时存活的敌军的数量最大值
#define ENEMY_NMAX           5

//BulletBox的容量
#define BOX_CAPACITY        100

//地图文件
#define MAP_FILE1           "map1.txt"
#define MAP_FILE2           "map2.txt"
#define MAP_FILE3           "map3.txt"
#define MAP_FILE1_NEW       "map1_new.txt"
#define MAP_FILE2_NEW       "map2_new.txt"
#define MAP_FILE3_NEW       "map3_new.txt"

