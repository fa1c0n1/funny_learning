// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 STUB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// STUB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef STUB_EXPORTS
#define STUB_API __declspec(dllexport)
#else
#define STUB_API __declspec(dllimport)
#endif

extern "C"
{
	typedef struct _GLOBAL_PARAM
	{
		DWORD       dwStart;               // Stub中的OEP
		BOOL        bShowMessage;          // 是否显示解密信息
		ULONG       dwImageBase;           // 映像基址
		DWORD       dwOEP;		           // 程序入口点
		PBYTE       lpStartVA;	           // 起始虚拟地址（被异或加密区）
		DWORD       dwCodeSize;	           // 被加密大小
		BYTE        byXor;                 // 加密异或的值
		DWORD       dwOriPeRelocRVA;       // 原始PE重定位表的RVA
		DWORD       dwRDataSectionRVA;     //.rdata段的RVA
		DWORD       dwRDataSectionSize;    //.rdata段的长度
		BYTE        bKey;                  //解密密钥
	}GLOBAL_PARAM, *PGLOBAL_PARAM;

	typedef ULONG(WINAPI *fnGetProcAddress)(_In_ HMODULE hModule, _In_ LPCSTR lpProcName);
	typedef HMODULE(WINAPI *fnLoadLibrary)(_In_ LPCTSTR lpLibFileName, HANDLE hFile, _In_ DWORD dwFlags);
	typedef int(WINAPI *fnMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	typedef HMODULE(WINAPI *fnGetModuleHandle)(_In_opt_ LPCTSTR lpModuleName);
	typedef BOOL(WINAPI *fnVirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);
	typedef void(WINAPI *fnExitProcess)(_In_ UINT uExitCode);
	typedef LRESULT(WINAPI *fnDefWindowProc)(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	typedef VOID(WINAPI *fnPostQuitMessage)(_In_ int nExitCode);
	typedef BOOL(WINAPI *fnSetWindowText)(_In_ HWND hWnd, _In_opt_ LPCSTR lpString);
	typedef int (WINAPI *fnGetWindowText)(_In_ HWND hWnd, _Out_ LPSTR lpString, _In_ int nMaxCount);
	typedef BOOL(WINAPI *fnShowWindow)(_In_ HWND hWnd, _In_ int nCmdShow);
	typedef BOOL(WINAPI *fnUpdateWindow)(HWND hWnd);
	typedef BOOL(WINAPI *fnGetMessage)(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax);
	typedef int(WINAPI *fnGetWindowTextLength)(_In_ HWND hWnd);
	typedef HWND(WINAPI *fnGetDlgItem)(_In_opt_ HWND hDlg, _In_ int nIDDlgItem);
	typedef LRESULT(WINAPI *fnDispatchMessage)(_In_ const MSG *lpmsg);
	typedef BOOL(WINAPI *fnTranslateMessage)(_In_ const MSG *lpMsg);
	typedef ATOM(WINAPI *fnRegisterClass)(_In_ const WNDCLASS *lpWndClass);

	typedef HWND(WINAPI *fnCreateWindowEx)(
		_In_      DWORD dwExStyle,
		_In_opt_  LPCTSTR lpClassName,
		_In_opt_  LPCTSTR lpWindowName,
		_In_      DWORD dwStyle,
		_In_      int x,
		_In_      int y,
		_In_      int nWidth,
		_In_      int nHeight,
		_In_opt_  HWND hWndParent,
		_In_opt_  HMENU hMenu,
		_In_opt_  HINSTANCE hInstance,
		_In_opt_  LPVOID lpParam
		);
	
	void Start();
}