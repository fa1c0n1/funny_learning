#include "DebuggerMain.h"
#include "stdafx.h"
#include "Comm.h"
#include "RegStruct.h"

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"

#pragma comment(lib, "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")


CDebuggerMain::CDebuggerMain()
	: m_bSystemBreakpoint(true), m_bUserTF(true)
{
}


CDebuggerMain::~CDebuggerMain()
{
}

void CDebuggerMain::launchDebugger()
{
	qout << qtr("请输入文件路径: ") << flush;
	m_strFile = qin.readLine();
	startDebug(m_strFile);
}


void CDebuggerMain::startDebug(QString strFile)
{
	if (strFile.isEmpty())
		return;

	STARTUPINFO stcStartupInfo = { 0 };
	stcStartupInfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION stcProcInfo = { 0 };

	TCHAR szFile[MAX_PATH] = {};
	strFile.toWCharArray(szFile);

	BOOL bRet = CreateProcess(
		szFile,
		NULL,
		NULL,
		NULL,
		FALSE,
		DEBUG_ONLY_THIS_PROCESS|CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&stcStartupInfo,
		&stcProcInfo);

	if (!bRet) {
		qout << qtr("创建调试进程失败") << endl;
		return;
	}

	DEBUG_EVENT debugEvent = { 0 };
	DWORD dwRet = DBG_CONTINUE;

	while (true) {
		//等待调试事件
		//  如果被调试进程产生了调试事件，函数就会
		//  将对应的信息输出到结构体变量中，并从函数中返回,
		//  如果被调试进程没有调试事件，函数会处于阻塞状态 
		WaitForDebugEvent(&debugEvent, INFINITE);

		switch (debugEvent.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			BREAKPOINT bp = {};
			setBreakpointCC(stcProcInfo.hProcess, debugEvent.u.CreateProcessInfo.lpStartAddress, &bp);
			m_vtBp.push_back(bp);
		}
			break;
		case EXCEPTION_DEBUG_EVENT:
			dwRet = onException(&debugEvent);
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			onProcessExited(&debugEvent.u.ExitProcess);
		default:
			break;
		}

		//回复调试事件的处理结果
		ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwRet);
	}
}

DWORD CDebuggerMain::onException(DEBUG_EVENT *pEvent)
{
	EXCEPTION_RECORD &er = pEvent->u.Exception.ExceptionRecord;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEvent->dwProcessId);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pEvent->dwThreadId);

	//将所有的断点都恢复
	setAllBreakpoint(hProcess);

	//被调试进程产生的第一个异常事件，这个
	//  异常事件就是系统断点
	qout << QString::asprintf("\t异常代码: %08X", er.ExceptionCode) << endl;
	qout << QString::asprintf("\t异常地址: %08X", er.ExceptionAddress) << endl;


	if (m_bSystemBreakpoint) {
		qout << qtr("到达系统断点") << endl;
		m_bSystemBreakpoint = false;
		userInput(hProcess, hThread);
		return DBG_CONTINUE;
	}

	//检查异常是否是调试器安装的断点引发的
	bool bFlag = false;
	for (auto &bp : m_vtBp) {
		if (bp.pAddr == er.ExceptionAddress) {
			bFlag = true;
			switch (bp.dwType)
			{
			case EXCEPTION_BREAKPOINT:
			{
				rmBreakpointCC(hProcess, hThread, bp.pAddr, bp.oldData);
				setBreakpointTF(hThread);
				m_bUserTF = false;
			}
			break;
			}
		}
	}

	//输出调试信息
	showDebugInfo(hProcess, hThread, er.ExceptionAddress);

	switch (er.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT: //软件断点

		break;
	case EXCEPTION_ACCESS_VIOLATION:  //内存访问异常(内存访问断点)
		break;

	//TF和硬件断点异常
	//  通过DR6寄存器进一步判断这个异常是
	//  TF引发的还是 DR0~DR3 引发的
	case EXCEPTION_SINGLE_STEP:
		if (!m_bUserTF) {
			goto _EXIT;
		}

		bFlag = true;
		break;
	default:
		break;
	}

	userInput(hProcess, hThread);

_EXIT:
	CloseHandle(hThread);
	CloseHandle(hProcess);

	if (!bFlag)
		return DBG_EXCEPTION_NOT_HANDLED;
	else
		return DBG_CONTINUE;
}

void CDebuggerMain::onProcessExited(EXIT_PROCESS_DEBUG_INFO *pEvent)
{
	qout << qtr("Debuggee was terminated.") << endl;
}

void CDebuggerMain::userInput(HANDLE hProcess, HANDLE hThread)
{
	// 接收用户控制
	// 1. 显示信息
	// 1.1 显示寄存器信息
	// 1.2 显示栈信息
	// 1.3 显示指定地址反汇编信息
	// 1.4 显示指定地址上的内存数据信息
	// 2. 程序控制
	// 2.1 单步步入
	// 2.2 下断点
	// 2.2.1 软件断点
	// 2.2.2 硬件断点
	// 2.2.3 内存访问断点
	// 2.3 直接运行
	QString strCmdLine;

	while (true) {
		qout << qtr(">> ") << flush;
		strCmdLine = qin.readLine();
		if (strCmdLine.isEmpty())
			continue;

		//单步步入命令
		QStringList cmdList = strCmdLine.split(' ', QString::SkipEmptyParts);
		
		if (cmdList[0] == qtr("t")) { //单步步入
			//TF断点原理:	
			//  当一个程序要运行指令时，CPU
			//  会检测EFLAGS的TF标志位是否为1
			//  如果是1，则CPU会主动产生一个硬件断点异常

			setBreakpointTF(hThread);

			m_bUserTF = true;
			break;
		}
		else if (cmdList[0] == qtr("bp")) {
			LPVOID pAddr = (LPVOID)cmdList[1].toULong(NULL, 16);
			BREAKPOINT bp = {};
			if (!setBreakpointCC(hProcess, pAddr, &bp)) {
				qout << qtr("设置断点失败") << endl;
			}
			else {
				m_vtBp.push_back(bp);
			}
		}
		else if (cmdList[0] == qtr("g")) {
			break;
		}
	}
}

void CDebuggerMain::showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr)
{
	//输出寄存器信息
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("获取线程上下文失败");

	qout << QString::asprintf("\tEAX:%08X ECX:%08X EDX:%08X EBX:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx) << endl;
	qout << QString::asprintf("\tEIP:%08X", ct.Eip) << endl;

	//输出栈信息
	DWORD dwBuf[5];
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, (LPCVOID)ct.Esp, dwBuf, sizeof(dwBuf), &bytesRead)) {
		DBGPRINT("读取进程内存失败");
	}
	else {
		qout << qtr("\t栈数据: \n");
		for (int i = 0; i < 5; i++) {
			qout << QString::asprintf("\t%08X|%08X", 
				ct.Esp + i * 4, dwBuf[i]) << endl;
		}
	}

	//输出反汇编信息
	LPBYTE opcode[200];
	if (!ReadProcessMemory(hProc, pExceptionAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("读取内存失败");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pExceptionAddr;
		disAsm.Archi = 0;  //x86汇编
		int nLen = 0;

		//nLen返回的是反汇编出来的指令的机器码字节数
		// 如果反汇编失败，则返回-1
		int nSize = 0;
		while (nSize < 5) {
			nLen = Disasm(&disAsm);
			if (nLen == -1)
				break;

			//输出反汇编
			qout << QString::asprintf("\t0x%08X|%s", 
				(DWORD)disAsm.VirtualAddr, disAsm.CompleteInstr) << endl;

			disAsm.EIP += nLen;
			disAsm.VirtualAddr += nLen;
			++nSize;
		}
	}
}

void CDebuggerMain::setAllBreakpoint(HANDLE hProcess)
{
	BREAKPOINT tBp = {};
	for (auto &bp : m_vtBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			setBreakpointCC(hProcess, bp.pAddr, &tBp);
		}
		else if (bp.dwType == EXCEPTION_SINGLE_STEP) {

		}
	}
}

bool CDebuggerMain::setBreakpointCC(HANDLE hProc, LPVOID pAddr, BREAKPOINT *bp)
{
	/*
	  原理:
	     在指定地址上写入 0xCC(int 3 指令的机器码),当
		 程序执行 int 3 指令的时候，就会产生异常，调试器就
		 能接收到该异常(也就是俗称的断下了)
	*/
		
	bp->pAddr = pAddr;
	bp->dwType = EXCEPTION_BREAKPOINT;

	//1.备份数据
	SIZE_T dwRead = 0;
	if (!ReadProcessMemory(hProc, pAddr, &bp->oldData, 1, &dwRead)) {
		DBGPRINT("读取进程内存失败");
		return false;
	}

	//2.写入CC
	if (!WriteProcessMemory(hProc, pAddr, "\xCC", 1, &dwRead)) {
		DBGPRINT("读取进程内存失败");
		return false;
	}

	return true;
}

//下一个单步步入的断点
void CDebuggerMain::setBreakpointTF(HANDLE hThread)
{
	//1.获取线程上下文
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_CONTROL;
	if (!GetThreadContext(hThread, &ct)) {
		DBGPRINT("获取线程环境失败");
	}

	//2.设置EFLAGS的TF标志位
	PEFLAGS pEflags = (PEFLAGS)&ct.EFlags;
	pEflags->TF = 1;

	//3.设置线程上下文
	if (!SetThreadContext(hThread, &ct)) {
		DBGPRINT("设置线程环境失败");
	}
}

bool CDebuggerMain::rmBreakpointCC(HANDLE hProc, HANDLE hThread, LPVOID pAddr, BYTE oldData)
{
	//1.直接将原始数据写入回去
	SIZE_T bytesWrite = 0;
	if (!WriteProcessMemory(hProc, pAddr, &oldData, 1, &bytesWrite)) {
		DBGPRINT("写入内存失败");
		return false;
	}

	//2.因为 int 3 是陷阱异常，断下之后，EIP是下一条指令的地址
	// 因此需要将 EIP-1
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_CONTROL;
	if (!GetThreadContext(hThread, &ct)) {
		DBGPRINT("获取线程环境失败");
		return false;
	}

	ct.Eip--;
	if (!SetThreadContext(hThread, &ct)) {
		DBGPRINT("获取线程环境失败");
		return false;
	}

	return true;
}
