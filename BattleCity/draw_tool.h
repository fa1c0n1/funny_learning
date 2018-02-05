#ifndef _DRAWTOOL_H
#define _DRAWTOOL_H

#include <windows.h>

extern void WriteChar(int nWidth, int nHeight, char *szChar, WORD wAttr);
extern void SetWindowSize(char *szTitle, int nX, int nY);

#endif