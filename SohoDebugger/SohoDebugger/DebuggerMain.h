#pragma once

#include "stdafx.h"
#include <TlHelp32.h>
#include <QVector>

//进程
typedef struct _PROCESS {
	DWORD dwPID;
	QString strProcName;
}PROCESS;

//软件断点
typedef struct _BREAKPOINT {
	LPVOID pAddr;
	bool bTmp;        //是否为临时断点
	DWORD dwType;     //断点的类型
	BYTE oldData;     //被int3断点覆盖的原始数据
}BREAKPOINT, *PBREAKPOINT;

//硬件断点
typedef struct _BREAKPOINTHARD {
	LPVOID pAddr;
	DWORD dwType;
	DWORD dwDrID; 
}BREAKPOINTHARD, *PBREAKPOINTHARD;

//内存断点
typedef struct _BREAKPOINTMEM {
	LPVOID pPageStartAddr;
	LPVOID pPageEndAddr;
	LPVOID pAddr;
	DWORD dwNewType;
	DWORD dwOldType;
}BREAKPOINTMEM, *PBREAKPOINTMEM;

typedef struct _EXECMODULE {
	QString strModPath;
	DWORD dwModBaseAddr;
	DWORD dwModSize;
}EXECMODULE, *PEXECMODULE;

typedef enum _HardBreakpointType {
	HARDBP_EXEC = 0,
	HARDBP_WRITE = 1,
	HARDBP_READWRITE = 3
}HardBreakpointType;

class CDebuggerMain
{
public:
	CDebuggerMain();
	~CDebuggerMain();

	void launchDebugger();

private:
	void showMainMenu();
	bool openProc(QString strFile);
	bool openProc(DWORD dwPID);

	void startDebug(QString strChoice);
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
	bool rmBreakpointCC(HANDLE hProc, LPVOID pAddr);
	void setBreakpointTF(HANDLE hThread);
	bool setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr, BREAKPOINTHARD *bph);
	bool setBreakpointHardRW(HANDLE hThread, ULONG_PTR pAddr, HardBreakpointType type, DWORD dwLen, BREAKPOINTHARD *bph);
	bool rmBreakpointHard(HANDLE hThread, ULONG_PTR pAddr);
	bool setBreakpointMem(HANDLE hProcess, ULONG_PTR pAddr, INT nType, BREAKPOINTMEM *bpm);
	bool rmBreakpointMem(HANDLE hProcess);
	bool resetBreakpointMem(HANDLE hProcess);
	bool isHardBreakpoint(HANDLE hThread);

	void showAllBreakpointCCInfo();
	void showAllBreakpointHardInfo();
	void showAllBreakpointMemInfo();


	void showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr);
	void showDisambleInfo(HANDLE hProc, LPVOID pAddr, int nCnt = 50);
	void showMemoryInfo(HANDLE hProc, LPVOID pAddr, int nNumOfBytesRead);
	void showStackInfo(HANDLE hProc, HANDLE hThread, int nCnt = 5);
	void showExecuteModuleInfo();
	void showRegisterInfo(HANDLE hThread);

	void editMemoryInfo(HANDLE hProc, LPVOID pAddr, QVector<BYTE> &vtData);
	void editRegister(HANDLE hThread, QString strRegName, DWORD dwVal);
	void writeAsmCode2Memory(HANDLE hProc, LPVOID pAddr, QString strAsmCode);

	void showAll32Process();
	void getAll32Process();
	bool getSeDebugPrivilge();

private:
	bool m_bAttach;
	bool m_bSystemBreakpoint;
	bool m_bUserTF;
	bool m_bGo;
	bool m_bTmpCC;
	bool m_bHaveBpMem;
	DWORD m_dwDebuggeePID;
	HANDLE m_hProcess;
	HANDLE m_hThread;
	QString m_strFile;
	QList<BREAKPOINT> m_listBp;
	QList<BREAKPOINTHARD> m_listBpHard;
	QList<BREAKPOINTMEM> m_listBpMem;
	QVector<EXECMODULE> m_vtModule;
	QVector<QString> m_vtRegName;
	QVector<PROCESS> m_vtProcess;
};

