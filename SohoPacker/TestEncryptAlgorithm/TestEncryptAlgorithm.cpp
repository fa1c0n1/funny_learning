// TestEncryptAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<Windows.h>

BYTE Encrypt(BYTE* lpBuffer, DWORD dwSize, BYTE bKey)
{
	for (DWORD d = 0; d < dwSize; ++d)
	{
		lpBuffer[d] = lpBuffer[d] + bKey;
		bKey = lpBuffer[d] ^ bKey;
	}
	return bKey;
}


void Decrypt(BYTE* lpBuffer, DWORD dwSize, BYTE bKey)
{
	for (signed int d = (dwSize - 1); d >= 0; --d)
	{
		bKey = lpBuffer[d] ^ bKey;
		lpBuffer[d] = lpBuffer[d] - bKey;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	BYTE chArr[64] = { "Welcome To Beijing" };//64

	printf("1: (%s)\n", chArr);
	BYTE Bb = 10;

	BYTE BRet = Encrypt((PBYTE)chArr, 64, Bb);
	printf("2: BRet=0x%02X, (%s)\n", BRet, chArr);

	Decrypt((PBYTE)chArr, 64, BRet);
	printf("3: (%s)\n", chArr);

	system("pause");
	return 0;
}

