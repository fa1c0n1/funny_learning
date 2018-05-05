#pragma once

#include "stdafx.h"
#include <QVector>

typedef struct _BREAKPOINT {
	LPVOID pAddr;
	DWORD dwType;     //断点的类型
	BYTE oldData;     //被int3断点覆盖的原始数据
}BREAKPOINT;

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
	void userInput(HANDLE hProcess, HANDLE hThread);
	void showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr);

	void setAllBreakpoint(HANDLE hProcess);
	bool setBreakpointCC(HANDLE hProc, LPVOID pAddr, BREAKPOINT *bp);
	void setBreakpointTF(HANDLE hThread);

	bool rmBreakpointCC(HANDLE hProc, HANDLE hThread, LPVOID pAddr, BYTE oldData);

private:
	bool m_bSystemBreakpoint;
	bool m_bUserTF;
	QString m_strFile;
	QVector<BREAKPOINT> m_vtBp;
};

