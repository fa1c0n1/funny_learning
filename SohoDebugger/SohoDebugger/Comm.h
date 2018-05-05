#pragma once

#define DBGPRINT(errMsg)  \
		qout << QString::asprintf("%s 中的函数 %s 第%d行错误: %s",\
			__FILE__,\
			__FUNCTION__,\
			__LINE__,\
			errMsg) << endl;

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