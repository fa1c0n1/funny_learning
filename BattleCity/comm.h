#ifndef _COMM_H
#define _COMM_H

#include <windows.h>

typedef unsigned char byte;
typedef unsigned int uint;

// 判断指定的按键是否按下
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// 判断指定的按键是否弹起
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif  // _COMM_H