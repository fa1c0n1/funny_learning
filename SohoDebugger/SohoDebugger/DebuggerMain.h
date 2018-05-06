#pragma once

#include "stdafx.h"
#include <TlHelp32.h>
#include <QVector>

typedef struct _BREAKPOINT {
	LPVOID pAddr;
	DWORD dwType;     //断点的类型
	BYTE oldData;     //被int3断点覆盖的原始数据
}BREAKPOINT, *PBREAKPOINT;

typedef struct _EXECMODULE {
	QString strModPath;
	DWORD dwModBaseAddr;
	DWORD dwModSize;
}EXECMODULE, *PEXECMODULE;

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

	void setAllBreakpoint(HANDLE hProcess);
	bool setBreakpointCC(HANDLE hProc, LPVOID pAddr, BREAKPOINT *bp);
	void setBreakpointTF(HANDLE hThread);

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
	DWORD m_dwDebuggeePID;
	HANDLE m_hProcess;
	HANDLE m_hThread;
	QString m_strFile;
	QVector<BREAKPOINT> m_vtBp;
	QVector<EXECMODULE> m_vtModule;
	QVector<QString> m_vtRegName;
};

