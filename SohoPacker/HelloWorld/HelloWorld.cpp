// HelloWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../ShPack/ShPack.h"
#pragma comment(lib,"../Release/ShPack.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	char szPath[MAX_PATH] = {};

	printf("路径: ");
	gets_s(szPath, MAX_PATH);
	CString strPath(szPath);

	if (Pack(strPath, (BYTE)0x15)) {
		printf("加壳成功\n");
	}

	system("pause");
	return 0;
}

