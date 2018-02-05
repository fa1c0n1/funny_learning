#ifndef _COMM_H
#define _COMM_H

#include <windows.h>

typedef unsigned char byte;
typedef unsigned int uint;

// �ж�ָ���İ����Ƿ���
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// �ж�ָ���İ����Ƿ���
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif  // _COMM_H