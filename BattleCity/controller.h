#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "data.h"

extern void LaunchGame(void);

int StartGame(void);
int LoadGame(void);
int EditMap(void);
int ExitGame(void);
void GoRun(void);
void FreeGameRes(void);

#endif  // _CONTROLLER_H