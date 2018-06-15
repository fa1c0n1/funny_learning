// ShStub.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ShStub.h"
#include <windows.h>
#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")
extern "C"__declspec(dllexport) GLOBAL_PARAM g_stcParam = { (DWORD)(Start) };

//----------- 全局变量 --------------------------
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

void InitData()
{
	// 获取kernel32基址
	g_pfnGetProcAddress = (fnGetProcAddress)MyGetProcAddress();
	ULONG dwBase = GetKernel32Addr();
	// 获取API地址
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
	// 弹出信息框
	int nRet = g_pfnMessageBox(NULL, L"欢迎使用免费加壳程序，是否运行主程序？", L"SohoPacker", MB_YESNO);
	if (nRet == IDYES)
	{
		// 修改代码段属性
		ULONG dwCodeBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.lpStartVA;

		DWORD dwOldCodeProtect = 0;
		g_pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldCodeProtect);
		XorCode(); // 解密代码

		// 修改.rdata段的属性
		DWORD dwOldRDataProtect = 0;
		ULONG dwRDataBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.dwRDataSectionRVA;
		g_pfnVirtualProtect((LPBYTE)dwRDataBase, g_stcParam.dwRDataSectionSize, PAGE_EXECUTE_READWRITE, &dwOldRDataProtect);

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
		g_oep(); // 跳回原始OEP
	}
	// 退出程序
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
	// 返回默认的窗口处理过程
	return g_pfnDefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ShowPasswordWindow()
{
	MSG msg = {};
	WNDCLASS wndcls = {};
	wndcls.lpszClassName = L"soho";
	wndcls.lpfnWndProc = MyWinProc;
	wndcls.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

	//注册窗口类
	g_pfnRegisterClass(&wndcls);

	//-------------创建主窗口---------------------
	HWND hWnd = g_pfnCreateWindowEx(
		0, L"soho", L"密码验证", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		500, 200, 500, 500, 
		NULL, NULL, NULL, NULL);

	//-------创建按钮------------------------------
	g_pfnCreateWindowEx(
		0, L"BUTTON", L"确定", WS_CHILD | WS_VISIBLE,
		200, 150,//在父窗口客户区的位置，
		100, 50,//宽,高
		hWnd,// 父窗口句柄
		(HMENU)0x1001,// 如果是顶层窗口 就是菜单句柄 子窗口就是本身的ID
		g_pfnGetModuleHandle(0), NULL);

	//-------创建编辑框--------------------------------
	DWORD dwStyle = ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | ES_PASSWORD;
	DWORD dwExStyle = WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	g_pfnCreateWindowEx(
		dwExStyle,            //扩展样式
		L"Edit",           //窗口类名
		L"",         //窗口标题
		dwStyle,              //窗口样式
		150,                  //x
		100,                  //y
		200,                  //宽度
		20,                   //高度
		hWnd,                 //父窗口句柄
		(HMENU)0x1002,        //ID
		g_pfnGetModuleHandle(0), //应用程序句柄
		NULL                  //附加参数
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



