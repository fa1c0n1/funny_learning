// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� STUB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// STUB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef STUB_EXPORTS
#define STUB_API __declspec(dllexport)
#else
#define STUB_API __declspec(dllimport)
#endif

extern "C"
{
	typedef struct _GLOBAL_PARAM
	{
		DWORD       dwStart;               // Stub�е�OEP
		BOOL        bShowMessage;          // �Ƿ���ʾ������Ϣ
		ULONG       dwImageBase;           // ӳ���ַ
		DWORD       dwOEP;		           // ������ڵ�
		PBYTE       lpStartVA;	           // ��ʼ�����ַ��������������
		DWORD       dwCodeSize;	           // �����ܴ�С
		BYTE        byXor;                 // ��������ֵ
		DWORD       dwOriPeRelocRVA;       // ԭʼPE�ض�λ���RVA
		DWORD       dwRDataSectionRVA;     //.rdata�ε�RVA
		DWORD       dwRDataSectionSize;    //.rdata�εĳ���
		BYTE        bKey;                  //������Կ
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