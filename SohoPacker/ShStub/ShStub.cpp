// ShStub.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ShStub.h"

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
		mov eax, dword ptr fs : [0x30] // eax = PEB�ĵ�ַ
		mov eax, [eax + 0x0C]          // eax = ָ��PEB_LDR_DATA�ṹ��ָ��
		mov eax, [eax + 0x1C]          // eax = ģ���ʼ�������ͷָ��InInitializationOrderModuleList
		mov eax, [eax]                 // eax = �б��еĵڶ�����Ŀ
		mov eax, [eax + 0x08]          // eax = ��ȡ����Kernel32.dll��ַ��Win7�»�ȡ����KernelBase.dll�Ļ�ַ��
		mov nKernel32Addr, eax
		pop eax
	}
	
	return nKernel32Addr;
}

ULONG MyGetProcAddress()
{
	ULONG nKernel32Base = GetKernel32Addr();
	// 1. ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)nKernel32Base;
	// 2. ��ȡNTͷ
	PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(nKernel32Base + pDosHeader->e_lfanew);
	// 3. ��ȡ����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pExpDir = 
		&pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	DWORD dwExpTableRVA = pExpDir->VirtualAddress;
	// 4. ��ȡ��������Ϣ�ṹ
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

		// 1. ��ȡ���
		DWORD dwID = pExpTable->Base + dwOrdinal;
		// 2. ��ȡ����������ַ
		ULONG dwFunAddrOffset = pEAT[dwOrdinal];

		for (DWORD dwIndex = 0; dwIndex < dwFunNameCount; dwIndex++)
		{
			// ����ű��в��Һ��������
			if (pEOT[dwIndex] == dwOrdinal)
			{
				// ��������������������Ʊ��е�����
				ULONG dwNameOffset = pENT[dwIndex];
				char* pFunName = (char*)(nKernel32Base + dwNameOffset);
				if (!strcmp(pFunName, "GetProcAddress"))
				{// ���ݺ������Ʒ��غ�����ַ
					return nKernel32Base + dwFunAddrOffset;
				}
			}
		}
	}
	return 0;
}

void XorCode()
{
	// ��ȡ�����ַ
	PBYTE pBase = (PBYTE)((ULONG)g_stcParam.dwImageBase + g_stcParam.lpStartVA);
	// ������
	for (DWORD i = 0; i < g_stcParam.dwCodeSize; i++)
	{
		pBase[i] ^= g_stcParam.byXor;
	}
}

void  Start()
{
	// ��ȡkernel32��ַ
	fnGetProcAddress pfnGetProcAddress = (fnGetProcAddress)MyGetProcAddress();
	ULONG dwBase = GetKernel32Addr();
	// ��ȡAPI��ַ
	fnLoadLibraryA pfnLoadLibraryA = (fnLoadLibraryA)pfnGetProcAddress((HMODULE)dwBase, "LoadLibraryExA");
	fnVirtualProtect pfnVirtualProtect = (fnVirtualProtect)pfnGetProcAddress((HMODULE)dwBase, "VirtualProtect");
	HMODULE hUser32 = (HMODULE)pfnLoadLibraryA("user32.dll");
	fnMessageBox pfnMessageBoxA = (fnMessageBox)pfnGetProcAddress(hUser32, "MessageBoxA");
	fnExitProcess pfnExitProcess = (fnExitProcess)pfnGetProcAddress((HMODULE)dwBase, "ExitProcess");

	// ������Ϣ��
	int nRet = pfnMessageBoxA(NULL, "��ӭʹ����Ѽӿǳ����Ƿ�����������", "Hello PEDIY", MB_YESNO);
	if (nRet == IDYES)
	{
		// �޸Ĵ��������
		ULONG dwCodeBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.lpStartVA;
		DWORD dwOldProtect = 0;
		pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		XorCode(); // ���ܴ���
		pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, dwOldProtect, &dwOldProtect);
		g_oep = (FUN)(g_stcParam.dwImageBase + g_stcParam.dwOEP);
		g_oep(); // ����ԭʼOEP
	}
	// �˳�����
	pfnExitProcess(0);
}

