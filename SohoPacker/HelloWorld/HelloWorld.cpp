// HelloWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../ShPack/ShPack.h"
#pragma comment(lib,"../Release/ShPack.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//CString strPath = "D:\\VSProjects\\SohoPacker\\Release\\test.exe";
	char szPath[MAX_PATH] = {};

	printf("·��: ");
	gets_s(szPath, MAX_PATH);
	CString strPath(szPath);

	if (Pack(strPath, (BYTE)0x15)) {
		printf("�ӿǳɹ�\n");
	}

	system("pause");
	return 0;
}

