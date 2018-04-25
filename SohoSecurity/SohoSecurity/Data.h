#pragma once

#include "stdafx.h"

typedef struct _Process2ModuleInfo {
	DWORD dwPID;
	QString strPName;
} PROCESS2MODULEINFO, *PPROCESS2MODULEINFO;

typedef struct _Process2ThreadInfo {
	DWORD dwPID;
	QString strPName;
} PROCESS2THREADINFO, *PPROCESS2THREADINFO;
