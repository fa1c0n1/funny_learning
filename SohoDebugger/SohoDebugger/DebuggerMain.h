#pragma once

#include "stdafx.h"
#include <TlHelp32.h>
#include <QVector>

typedef struct _BREAKPOINT {
	LPVOID pAddr;
	bool bTmp;        //�Ƿ�Ϊ��ʱ�ϵ�
	DWORD dwType;     //�ϵ������
	BYTE oldData;     //��int3�ϵ㸲�ǵ�ԭʼ����
}BREAKPOINT, *PBREAKPOINT;

typedef struct _EXECMODULE {
	QString strModPath;
	DWORD dwModBaseAddr;
	DWORD dwModSize;
}EXECMODULE, *PEXECMODULE;

typedef enum _HardBreakpointType {
	HARDBP_EXEC = 0,
	HARDBP_READ = 1,
	HARDBP_WRITE = 2
}HardBreakpointType;

class CDebuggerMain
{
public:
	CDebuggerMain();
	~CDebuggerMain();

	void launchDebugger();

private:
	void startDebug(QString strFile);
	DWORD onException(DEBUG_EVENT *pEvent);
	void onProcessExited(EXIT_PROCESS_DEBUG_INFO *pEvent);
	//DWORD dispatchException(EXCEPTION_DEBUG_INFO *pInfo);
	void userInput(HANDLE hProcess, HANDLE hThread, LPVOID pExceptionAddr);
	void traverseExecModule(DWORD dwPID);
	void getNextInstructAddr(HANDLE hProcess, LPVOID pAddr, LPVOID *pNextAddr);
	void getDebuggeeContext(PCONTEXT pContext);
	int isCallInstruction(DWORD dwAddr);
	void setDebuggeeContext(PCONTEXT pContext);

	void resetAllBreakpoint(HANDLE hProcess);
	void clearAllBreakpoint(HANDLE hProcess);
	bool setBreakpointCC(HANDLE hProc, LPVOID pAddr, BREAKPOINT *bp);
	void setBreakpointTF(HANDLE hThread);
	bool setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr);
	bool setBreakpointHardRW(HANDLE hThread, ULONG_PTR pAddr, HardBreakpointType type, DWORD dwLen);
	bool isHardBreakpoint(HANDLE hThread);

	bool rmBreakpointCC(HANDLE hProc, HANDLE hThread, LPVOID pAddr, BYTE oldData);

	void showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr);
	void showDisambleInfo(HANDLE hProc, LPVOID pAddr, int nCnt = 50);
	void showMemoryInfo(HANDLE hProc, LPVOID pAddr, int nNumOfBytesRead);
	void showStackInfo(HANDLE hProc, HANDLE hThread, int nCnt = 5);
	void showExecuteModuleInfo();
	void showRegisterInfo(HANDLE hThread);

	void editMemoryInfo(HANDLE hProc, LPVOID pAddr, QVector<BYTE> &vtData);
	void editRegister(HANDLE hThread, QString strRegName, DWORD dwVal);
	void writeAsmCode2Memory(HANDLE hProc, LPVOID pAddr, QString strAsmCode);


private:
	bool m_bSystemBreakpoint;
	bool m_bUserTF;
	bool m_bGo;
	bool m_bTmpCC;
	DWORD m_dwDebuggeePID;
	HANDLE m_hProcess;
	HANDLE m_hThread;
	QString m_strFile;
	QList<BREAKPOINT> m_listBp;
	QVector<EXECMODULE> m_vtModule;
	QVector<QString> m_vtRegName;
};

