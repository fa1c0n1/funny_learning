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
void InitGameRoles(void);
void ShowMenu(void);
int ShowSubMenu(void);
int GoOnGame(void);
int SaveGame(void);
int RetMainMenu(void);
void ShowFailedNotice(void);
void ShowWinNotice(void);
void ShowTODO(void);

#endif  // _CONTROLLER_H