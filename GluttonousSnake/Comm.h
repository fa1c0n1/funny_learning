#pragma once

//游戏级别
typedef enum {
	GL_EASY,
	GL_NORMAL,
	GL_HARD,
	GL_RECORD,
} GameLevel;

//移动方向
typedef enum {
	DRT_UP,
	DRT_DOWN,
	DRT_LEFT,
	DRT_RIGHT
} Direction;

typedef enum {
	SMOP_CONTINUE,
	SMOP_RESTART,
	SMOP_SAVE,
	SMOP_QUIT
} SubMenuOption;

typedef enum {
	MAPCUST_RET,
	MAPCUST_YES,
	MAPCUST_NO
} MapOption;

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
#define KEY_ENTER           13
#define KEY_EXIT            27
#define KEY_UP              72
#define KEY_DOWN            80
#define KEY_LEFT            75
#define KEY_RIGHT           77
#define KEY_DIRECTION       224

// 蛇的初始生命值
#define SNAKE_INIT_LIFE     5

// 存档文件
#define RECORD_FILE         "GameRecord.bin"


// 存档缓冲区
#define RDBUF_LEN           4096