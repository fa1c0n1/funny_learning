// ShStub.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ShStub.h"
#include <windows.h>
#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")
extern "C"__declspec(dllexport) GLOBAL_PARAM g_stcParam = { (DWORD)(Start) };

//----------- ȫ�ֱ��� --------------------------
typedef void(*FUN)();
FUN g_oep;

fnGetProcAddress g_pfnGetProcAddress = NULL;
fnLoadLibrary g_pfnLoadLibrary = NULL;
fnVirtualProtect g_pfnVirtualProtect = NULL;
fnGetModuleHandle g_pfnGetModuleHandle = NULL;
fnMessageBox g_pfnMessageBox = NULL;
fnExitProcess g_pfnExitProcess = NULL;
fnDefWindowProc g_pfnDefWindowProc = NULL;
fnPostQuitMessage g_pfnPostQuitMessage = NULL;
fnSetWindowText g_pfnSetWindowText = NULL;
fnGetWindowText g_pfnGetWindowText = NULL;
fnShowWindow g_pfnShowWindow = NULL;
fnUpdateWindow g_pfnUpdateWindow = NULL;
fnGetMessage g_pfnGetMessage = NULL;
fnGetWindowTextLength g_pfnGetWindowTextLength = NULL;
fnGetDlgItem g_pfnGetDlgItem = NULL;
fnDispatchMessage g_pfnDispatchMessage = NULL;
fnTranslateMessage g_pfnTranslateMessage = NULL;
fnRegisterClass g_pfnRegisterClass = NULL;
fnCreateWindowEx g_pfnCreateWindowEx = NULL;

CHAR g_szPwdText[64] = {};
DWORD g_dwDefaultImageBase = 0;

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
	//char szFuncName[15] = { 'G', 'e', 't', 'P', 'r', 'o', 'c', 'A', 'd', 'd', 'r', 'e', 's', 's', '\0' };
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

void InitData()
{
	// ��ȡkernel32��ַ
	g_pfnGetProcAddress = (fnGetProcAddress)MyGetProcAddress();
	ULONG dwBase = GetKernel32Addr();
	// ��ȡAPI��ַ
	g_pfnLoadLibrary = (fnLoadLibrary)g_pfnGetProcAddress((HMODULE)dwBase, "LoadLibraryExW");
	g_pfnVirtualProtect = (fnVirtualProtect)g_pfnGetProcAddress((HMODULE)dwBase, "VirtualProtect");
	g_pfnGetModuleHandle = (fnGetModuleHandle)g_pfnGetProcAddress((HMODULE)dwBase, "GetModuleHandleW");
	HMODULE hUser32 = (HMODULE)g_pfnLoadLibrary(L"user32.dll", NULL, NULL);
	g_pfnMessageBox = (fnMessageBox)g_pfnGetProcAddress(hUser32, "MessageBoxW");
	g_pfnGetWindowText = (fnGetWindowText)g_pfnGetProcAddress(hUser32, "GetWindowTextA");
	g_pfnExitProcess = (fnExitProcess)g_pfnGetProcAddress((HMODULE)dwBase, "ExitProcess");
	g_pfnDefWindowProc = (fnDefWindowProc)g_pfnGetProcAddress(hUser32, "DefWindowProcW");
	g_pfnPostQuitMessage = (fnPostQuitMessage)g_pfnGetProcAddress(hUser32, "PostQuitMessage");
	g_pfnSetWindowText = (fnSetWindowText)g_pfnGetProcAddress(hUser32, "SetWindowTextA");
	g_pfnShowWindow = (fnShowWindow)g_pfnGetProcAddress(hUser32, "ShowWindow");
	g_pfnUpdateWindow = (fnUpdateWindow)g_pfnGetProcAddress(hUser32, "UpdateWindow");
	g_pfnGetMessage = (fnGetMessage)g_pfnGetProcAddress(hUser32, "GetMessageW");
	g_pfnGetWindowTextLength = (fnGetWindowTextLength)g_pfnGetProcAddress(hUser32, "GetWindowTextLengthA");
	g_pfnGetDlgItem = (fnGetDlgItem)g_pfnGetProcAddress(hUser32, "GetDlgItem");
	g_pfnDispatchMessage = (fnDispatchMessage)g_pfnGetProcAddress(hUser32, "DispatchMessageW");
	g_pfnTranslateMessage = (fnTranslateMessage)g_pfnGetProcAddress(hUser32, "TranslateMessage");
	g_pfnRegisterClass = (fnRegisterClass)g_pfnGetProcAddress(hUser32, "RegisterClassW");
	g_pfnCreateWindowEx = (fnCreateWindowEx)g_pfnGetProcAddress(hUser32, "CreateWindowExW");
}

void UnPack()
{
	// ������Ϣ��
	int nRet = g_pfnMessageBox(NULL, L"��ӭʹ����Ѽӿǳ����Ƿ�����������", L"SohoPacker", MB_YESNO);
	if (nRet == IDYES)
	{
		// �޸Ĵ��������
		ULONG dwCodeBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.lpStartVA;

		DWORD dwOldCodeProtect = 0;
		g_pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldCodeProtect);
		XorCode(); // ���ܴ���

		// �޸�.rdata�ε�����
		DWORD dwOldRDataProtect = 0;
		ULONG dwRDataBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.dwRDataSectionRVA;
		g_pfnVirtualProtect((LPBYTE)dwRDataBase, g_stcParam.dwRDataSectionSize, PAGE_EXECUTE_READWRITE, &dwOldRDataProtect);

		//---------------�޸�ԭexe���ض�λ��Ϣ-----------------------------------------
		DWORD dwOriPeRelocSize = 0;
		if (g_stcParam.dwOriPeRelocRVA != 0) {
			PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)(g_stcParam.dwImageBase + g_stcParam.dwOriPeRelocRVA);
			typedef struct {
				WORD Offset : 12;          // (1) ��СΪ12Bit���ض�λƫ��
				WORD Type : 4;             // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
			}TypeOffset, *PTypeOffset;
			// ѭ����ȡÿһ��MAGE_BASE_RELOCATION�ṹ���ض�λ��Ϣ
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
					ULONG dwRelocData = *pRelocAddr - g_dwDefaultImageBase + g_stcParam.dwImageBase;
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
		g_pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, dwOldCodeProtect, &dwTmp);
		g_pfnVirtualProtect((LPBYTE)dwRDataBase, g_stcParam.dwRDataSectionSize, dwOldRDataProtect, &dwTmp);
		g_oep = (FUN)(g_stcParam.dwImageBase + g_stcParam.dwOEP);
		g_oep(); // ����ԭʼOEP
	}
	// �˳�����
	g_pfnExitProcess(0);
}

LRESULT CALLBACK MyWinProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE: {
		return 0;
	}
	case WM_COMMAND: {
		WORD wId = LOWORD(wParam);
		WORD wCode = HIWORD(wParam);
		HANDLE hChild = (HANDLE)lParam;

		if (wId == 0x1001 && wCode == BN_CLICKED) {	
			HWND hPwdEdit = g_pfnGetDlgItem(hwnd, 0x1002);
			g_pfnGetWindowText(hPwdEdit, g_szPwdText, 64);
			g_pfnShowWindow(hwnd, SW_HIDE);
			UnPack();
			return 0;
		}
		break;
	}
	case WM_CLOSE: {
		g_pfnExitProcess(0);
		return 0;
	}

	}
	// ����Ĭ�ϵĴ��ڴ������
	return g_pfnDefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ShowPasswordWindow()
{
	MSG msg = {};
	WNDCLASS wndcls = {};
	wndcls.lpszClassName = L"soho";
	wndcls.lpfnWndProc = MyWinProc;
	wndcls.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

	//ע�ᴰ����
	g_pfnRegisterClass(&wndcls);

	//-------------����������---------------------
	HWND hWnd = g_pfnCreateWindowEx(
		0, L"soho", L"������֤", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		500, 200, 500, 500, 
		NULL, NULL, NULL, NULL);

	//-------������ť------------------------------
	g_pfnCreateWindowEx(
		0, L"BUTTON", L"ȷ��", WS_CHILD | WS_VISIBLE,
		200, 150,//�ڸ����ڿͻ�����λ�ã�
		100, 50,//��,��
		hWnd,// �����ھ��
		(HMENU)0x1001,// ����Ƕ��㴰�� ���ǲ˵���� �Ӵ��ھ��Ǳ����ID
		g_pfnGetModuleHandle(0), NULL);

	//-------�����༭��--------------------------------
	DWORD dwStyle = ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | ES_PASSWORD;
	DWORD dwExStyle = WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	g_pfnCreateWindowEx(
		dwExStyle,            //��չ��ʽ
		L"Edit",           //��������
		L"",         //���ڱ���
		dwStyle,              //������ʽ
		150,                  //x
		100,                  //y
		200,                  //���
		20,                   //�߶�
		hWnd,                 //�����ھ��
		(HMENU)0x1002,        //ID
		g_pfnGetModuleHandle(0), //Ӧ�ó�����
		NULL                  //���Ӳ���
		);

	g_pfnShowWindow(hWnd, SW_SHOW);
	g_pfnUpdateWindow(hWnd);
	while (g_pfnGetMessage(&msg, 0, 0, 0)) {
		g_pfnTranslateMessage(&msg);
		g_pfnDispatchMessage(&msg);
	}
}

void  Start()
{
	InitData();
	g_dwDefaultImageBase = g_stcParam.dwImageBase;
	g_stcParam.dwImageBase = (ULONG)g_pfnGetModuleHandle(NULL);
	ShowPasswordWindow();
}



