// SohoPacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../ShPack/ShPack.h"
#pragma comment(lib,"../Release/ShPack.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//TCHAR szPath[] = L"test.exe";
	//CString szPath = "D:\\VSProjects\\SohoPacker\\Release\\test.exe";

	

	if (Pack(szPath, (BYTE)0x15)) {
		printf("¼Ó¿Ç³É¹¦\n");
	}

	system("pause");
	return 0;
}

