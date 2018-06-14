// ShStub.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ShStub.h"
#include <windows.h>
#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")
extern "C"__declspec(dllexport) GLOBAL_PARAM g_stcParam = { (DWORD)(Start) };

typedef void(*FUN)();
FUN g_oep;

ULONG GetKernel32Addr()
{
	ULONG nKernel32Addr = 0;

	__asm {
		push eax
		mov eax, dword ptr fs : [0x30] // eax = PEB的地址
		mov eax, [eax + 0x0C]          // eax = 指向PEB_LDR_DATA结构的指针
		mov eax, [eax + 0x1C]          // eax = 模块初始化链表的头指针InInitializationOrderModuleList
		mov eax, [eax]                 // eax = 列表中的第二个条目
		mov eax, [eax + 0x08]          // eax = 获取到的Kernel32.dll基址（Win7下获取的是KernelBase.dll的基址）
		mov nKernel32Addr, eax
		pop eax
	}
	
	return nKernel32Addr;
}

ULONG MyGetProcAddress()
{
	ULONG nKernel32Base = GetKernel32Addr();
	//char szFuncName[15] = { 'G', 'e', 't', 'P', 'r', 'o', 'c', 'A', 'd', 'd', 'r', 'e', 's', 's', '\0' };
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)nKernel32Base;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(nKernel32Base + pDosHeader->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pExpDir = 
		&pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	DWORD dwExpTableRVA = pExpDir->VirtualAddress;
	// 4. 获取导出表信息结构
	PIMAGE_EXPORT_DIRECTORY pExpTable = (PIMAGE_EXPORT_DIRECTORY)(nKernel32Base + dwExpTableRVA);
	DWORD dwFunCount = pExpTable->NumberOfFunctions;
	DWORD dwFunNameCount = pExpTable->NumberOfNames;
	DWORD dwModOffset = pExpTable->Name;

	// Get Export Address Table
	PDWORD pEAT = (PDWORD)(nKernel32Base + pExpTable->AddressOfFunctions);
	// Get Export Name Table
	PDWORD pENT = (PDWORD)(nKernel32Base + pExpTable->AddressOfNames);
	// Get Export Index Table
	PWORD  pEOT = (PWORD)(nKernel32Base + pExpTable->AddressOfNameOrdinals);

	for (DWORD dwOrdinal = 0; dwOrdinal < dwFunCount; dwOrdinal++)
	{
		if (!pEAT[dwOrdinal]) // Export Address offset
			continue;

		// 1. 获取序号
		DWORD dwID = pExpTable->Base + dwOrdinal;
		// 2. 获取导出函数地址
		ULONG dwFunAddrOffset = pEAT[dwOrdinal];

		for (DWORD dwIndex = 0; dwIndex < dwFunNameCount; dwIndex++)
		{
			// 在序号表中查找函数的序号
			if (pEOT[dwIndex] == dwOrdinal)
			{
				// 根据序号索引到函数名称表中的名字
				ULONG dwNameOffset = pENT[dwIndex];
				char* pFunName = (char*)(nKernel32Base + dwNameOffset);
				if (!strcmp(pFunName, "GetProcAddress"))
				{// 根据函数名称返回函数地址
					return nKernel32Base + dwFunAddrOffset;
				}
			}
		}
	}
	return 0;
}

void XorCode()
{
	// 获取代码基址
	PBYTE pBase = (PBYTE)((ULONG)g_stcParam.dwImageBase + g_stcParam.lpStartVA);
	// 异或操作
	for (DWORD i = 0; i < g_stcParam.dwCodeSize; i++)
	{
		pBase[i] ^= g_stcParam.byXor;
	}
}

void  Start()
{
	// 获取kernel32基址
	fnGetProcAddress pfnGetProcAddress = (fnGetProcAddress)MyGetProcAddress();
	ULONG dwBase = GetKernel32Addr();
	// 获取API地址
	fnLoadLibraryA pfnLoadLibraryA = (fnLoadLibraryA)pfnGetProcAddress((HMODULE)dwBase, "LoadLibraryExA");
	fnVirtualProtect pfnVirtualProtect = (fnVirtualProtect)pfnGetProcAddress((HMODULE)dwBase, "VirtualProtect");
	fnGetModuleHandleA pfnGetModuleHandleA = (fnGetModuleHandleA)pfnGetProcAddress((HMODULE)dwBase, "GetModuleHandleA");
	DWORD dwDefaultImageBase = g_stcParam.dwImageBase;
	g_stcParam.dwImageBase = (ULONG)pfnGetModuleHandleA(NULL);
	HMODULE hUser32 = (HMODULE)pfnLoadLibraryA("user32.dll");
	fnMessageBox pfnMessageBoxA = (fnMessageBox)pfnGetProcAddress(hUser32, "MessageBoxA");
	fnExitProcess pfnExitProcess = (fnExitProcess)pfnGetProcAddress((HMODULE)dwBase, "ExitProcess");

	// 弹出信息框
	int nRet = pfnMessageBoxA(NULL, "欢迎使用免费加壳程序，是否运行主程序？", "Hello PEDIY", MB_YESNO);
	if (nRet == IDYES)
	{
		// 修改代码段属性
		ULONG dwCodeBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.lpStartVA;

		DWORD dwOldCodeProtect = 0;
		pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldCodeProtect);
		XorCode(); // 解密代码

		// 修改.rdata段的属性
		DWORD dwOldRDataProtect = 0;
		ULONG dwRDataBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.dwRDataSectionRVA;
		pfnVirtualProtect((LPBYTE)dwRDataBase, g_stcParam.dwRDataSectionSize, PAGE_EXECUTE_READWRITE, &dwOldRDataProtect);

		//---------------修复原exe的重定位信息-----------------------------------------
		DWORD dwOriPeRelocSize = 0;
		if (g_stcParam.dwOriPeRelocRVA != 0) {
			PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)(g_stcParam.dwImageBase + g_stcParam.dwOriPeRelocRVA);
			typedef struct {
				WORD Offset : 12;          // (1) 大小为12Bit的重定位偏移
				WORD Type : 4;             // (2) 大小为4Bit的重定位信息类型值
			}TypeOffset, *PTypeOffset;
			// 循环获取每一个MAGE_BASE_RELOCATION结构的重定位信息
			while (pRelocBlock->VirtualAddress)
			{
				dwOriPeRelocSize += pRelocBlock->SizeOfBlock;
				PTypeOffset pTypeOffset = (PTypeOffset)(pRelocBlock + 1);
				ULONG dwCount = (pRelocBlock->SizeOfBlock - 8) / 2;
				for (ULONG i = 0; i < dwCount; i++)
				{
					if (*(WORD*)&pTypeOffset[i] == 0)
						continue;

					ULONG dwRVA = pRelocBlock->VirtualAddress + pTypeOffset[i].Offset;
					PULONG pRelocAddr = (PULONG)(g_stcParam.dwImageBase + dwRVA);
					ULONG dwRelocData = *pRelocAddr - dwDefaultImageBase + g_stcParam.dwImageBase;
					*pRelocAddr = dwRelocData;
				}
				pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG)pRelocBlock + pRelocBlock->SizeOfBlock);
			}

			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)g_stcParam.dwImageBase;
			PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(g_stcParam.dwImageBase + pDosHeader->e_lfanew);
			PIMAGE_DATA_DIRECTORY pRelocDir = (PIMAGE_DATA_DIRECTORY)&pNtHeader->OptionalHeader.DataDirectory[5];
		}
		//--------------------------------------------------------------------

		DWORD dwTmp = 0;
		pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, dwOldCodeProtect, &dwTmp);
		pfnVirtualProtect((LPBYTE)dwRDataBase, g_stcParam.dwRDataSectionSize, dwOldRDataProtect, &dwTmp);
		g_oep = (FUN)(g_stcParam.dwImageBase + g_stcParam.dwOEP);
		g_oep(); // 跳回原始OEP
	}
	// 退出程序
	pfnExitProcess(0);
}



