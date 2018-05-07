#include "DebuggerMain.h"
#include "stdafx.h"
#include "Comm.h"
#include "RegStruct.h"
#include <TlHelp32.h>

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"
#include "keystone/keystone.h"

#
#pragma comment (lib,"keystone/x86/keystone_x86.lib")
#pragma comment(lib, "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")

CDebuggerMain::CDebuggerMain()
	: m_bSystemBreakpoint(true), m_bUserTF(true),
	m_bGo(false), m_bTmpCC(false)
{
	m_vtRegName.push_back(qtr("EAX"));
	m_vtRegName.push_back(qtr("ECX"));
	m_vtRegName.push_back(qtr("EDX"));
	m_vtRegName.push_back(qtr("EBX"));
	m_vtRegName.push_back(qtr("EBP"));
	m_vtRegName.push_back(qtr("ESP"));
	m_vtRegName.push_back(qtr("ESI"));
	m_vtRegName.push_back(qtr("EDI"));
	m_vtRegName.push_back(qtr("EIP"));
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

	m_dwDebuggeePID = stcProcInfo.dwProcessId;

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
			m_listBp.push_back(bp);
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

void CDebuggerMain::traverseExecModule(DWORD dwPID)
{
	HANDLE hModuleSnap = 0;
	MODULEENTRY32 me32 = {};
	QString strModuleBaseAddr;
	QString strModuleSize;
	EXECMODULE execModule = {};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	me32.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hModuleSnap, &me32)) {
		//TODO:
		CloseHandle(hModuleSnap);
		return;
	}
	else {
		do
		{
			execModule.strModPath = QString::fromWCharArray(me32.szExePath);
			execModule.dwModBaseAddr = (DWORD)me32.modBaseAddr;
			execModule.dwModSize = me32.modBaseSize;
			m_vtModule.push_back(execModule);
		} while (Module32Next(hModuleSnap, &me32));

		CloseHandle(hModuleSnap);
	}
}

DWORD CDebuggerMain::onException(DEBUG_EVENT *pEvent)
{
	DWORD dwRet = DBG_CONTINUE;
	EXCEPTION_RECORD &er = pEvent->u.Exception.ExceptionRecord;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEvent->dwProcessId);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pEvent->dwThreadId);

	m_hThread = hThread;
	m_hProcess = hProcess;

	//被调试进程产生的第一个异常事件，这个
	//  异常事件就是系统断点
	qout << QString::asprintf("\t异常代码: %08X", er.ExceptionCode) << endl;
	qout << QString::asprintf("\t异常地址: %08X", er.ExceptionAddress) << endl;

	if (m_bSystemBreakpoint) {
		qout << qtr("到达系统断点,忽略") << endl;
		m_bSystemBreakpoint = false;
		return DBG_CONTINUE;
	}

	switch (er.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT: //软件断点
	{
		clearAllBreakpoint(hProcess);
		//输出调试信息
		showDebugInfo(hProcess, hThread, er.ExceptionAddress);
		if (m_bTmpCC)
			m_listBp.pop_back();
		CONTEXT ct = {};
		getDebuggeeContext(&ct);
		ct.Eip--;
		setDebuggeeContext(&ct);
	}
		break;
	case EXCEPTION_ACCESS_VIOLATION:  //内存访问异常(内存访问断点)
		break;

	//TF和硬件断点异常
	//  通过DR6寄存器进一步判断这个异常是
	//  TF引发的还是 DR0~DR3 引发的
	case EXCEPTION_SINGLE_STEP:
		//输出调试信息
		showDebugInfo(hProcess, hThread, er.ExceptionAddress);

		if (isHardBreakpoint(hThread)) {
			break;
		}
		
		//将所有的断点都恢复
		resetAllBreakpoint(hProcess);
		if (m_bGo) {
			goto _EXIT;
		}

		
		break;
	default: 
		dwRet = DBG_EXCEPTION_NOT_HANDLED;
		goto _EXIT;
		break;
	}

	userInput(hProcess, hThread, er.ExceptionAddress);

_EXIT:
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return dwRet;
}

void CDebuggerMain::onProcessExited(EXIT_PROCESS_DEBUG_INFO *pEvent)
{
	qout << qtr("Debuggee was terminated.") << endl;
}

void CDebuggerMain::userInput(HANDLE hProcess, HANDLE hThread, LPVOID pExceptionAddr)
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

	m_bGo = false;
	m_bTmpCC = false;
	while (true) {
		qout << qtr(">> ") << flush;
		strCmdLine = qin.readLine();
		if (strCmdLine.isEmpty())
			continue;

		QStringList cmdList = strCmdLine.split(' ', QString::SkipEmptyParts);
		
		if (cmdList[0] == qtr("t")) { //t:单步步入
			//TF断点原理:	
			//  当一个程序要运行指令时，CPU
			//  会检测EFLAGS的TF标志位是否为1
			//  如果是1，则CPU会主动产生一个硬件断点异常

			setBreakpointTF(hThread);
			//m_bUserTF = true;
			break;
		}
		else if (cmdList[0] == qtr("n")) { //n:单步步过
			//先判断当前EIP指向的指令是否为CALL指令
			CONTEXT tContext;
			getDebuggeeContext(&tContext);
			int nCallLen = isCallInstruction(tContext.Eip);

			if (nCallLen) { //是call指令，则在下一条指令处设置软件断点
				BREAKPOINT bp = {};
				if (!setBreakpointCC(hProcess, (LPVOID)(tContext.Eip + nCallLen), &bp)) {
					DBGPRINT("设置断点失败");
				}
				else {
					bp.bTmp = true;
					m_bTmpCC = true;
					m_listBp.push_back(bp);
					break;
				}
			}
			else { //不是call指令，则设置TF
				setBreakpointTF(hThread);
				//m_bUserTF = true;
				break;
			}
		}
		else if (cmdList[0] == qtr("b")) {  //b:设置软件断点
			if (cmdList.size() < 2) {
				qout << qtr("缺少地址") << endl;
			}
			else if (cmdList.size() < 3) {
				bool bOK = true;
				LPVOID pAddr = (LPVOID)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					BREAKPOINT bp = {};
					if (!setBreakpointCC(hProcess, pAddr, &bp)) {
						DBGPRINT("设置断点失败");
					}
					else {
						m_listBp.push_back(bp);
					}
				}
				else {
					qout << qtr("%1不是一个合法的16进制值").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bh")) { //bh:设置硬件断点
			if (cmdList.size() < 2) {
				qout << qtr("缺少地址") << endl;
			}
			else if (cmdList.size() < 3) {
				bool bOK = true;
				ULONG_PTR pAddr = (ULONG_PTR)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					if (!setBreakpointHardExec(hThread, pAddr))
						DBGPRINT("设置硬件执行断点失败");
				}
				else {
					qout << qtr("%1不是一个合法的16进制值").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bm")) { //bm:设置内存断点

		}
		else if (cmdList[0] == qtr("lm")) { //lm:查看可执行模块信息
			showExecuteModuleInfo();
		}
		else if (cmdList[0] == qtr("r")) { //r: 查看/修改寄存器
			if (cmdList.size() < 2) { //r 查看所有寄存器的值
				showRegisterInfo(hThread);
			}
			else if (cmdList.size() < 3) { //r [reg] 修改寄存器的值
				QString strRegName = cmdList[1].toUpper();
				if (m_vtRegName.contains(strRegName)) {
					QString strRegVal;
					DWORD dwRegVal;
					qout << strRegName << qtr(":") << flush;
					strRegVal = qin.readLine();
					bool bOK = true;
					dwRegVal = strRegVal.toULong(&bOK, 16);
					if (bOK) {
						editRegister(hThread, strRegName, dwRegVal);
					}
					else {
						qout << qtr("%1 不是合法的16进制值").arg(strRegVal) << endl;
					}
				}
				else {
					qout << qtr("%1:不存在该寄存器").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("u")) { //u:查看反汇编代码
			if (cmdList.size() < 2) {
				showDisambleInfo(hProcess, pExceptionAddr, 50);
			}
			else {
				bool bOK = true;
				int nLineNum = cmdList[1].toInt(&bOK, 10);
				if (bOK)
					showDisambleInfo(hProcess, pExceptionAddr, nLineNum);
				else
					showDisambleInfo(hProcess, pExceptionAddr, 5);
			}
		}
		else if (cmdList[0] == qtr("d")) { //d:查看内存数据
			if (cmdList.size() < 2) {
				showMemoryInfo(hProcess, pExceptionAddr, 128);
			}
			else if (cmdList.size() < 3) { //d [addr]
				bool bOK = true;
				DWORD dwAddr = cmdList[1].toULong(&bOK, 16);
				if (bOK)
					showMemoryInfo(hProcess, (LPVOID)dwAddr, 128);
				else
					qout << qtr("输入的地址不合法") << endl;
			}
			else { //d [addr] [nNumOfBytes]
				bool bOK = true;
				DWORD dwAddr = cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					int nBytesOfRead = cmdList[2].toInt(&bOK, 10);
					if (bOK) {
						if (nBytesOfRead <= 0 || nBytesOfRead > 1024)
							showMemoryInfo(hProcess, (LPVOID)dwAddr, 1024);
						else
							showMemoryInfo(hProcess, (LPVOID)dwAddr, nBytesOfRead);
					}
				}
				else {
					qout << qtr("输入的地址不合法") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("e")) { //e:修改内存中的数据
			if (cmdList.size() > 3) {
				bool bOK = true;
				DWORD dwAddr = cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					QVector<BYTE> vtData;
					for (int nIdx = 2; nIdx < cmdList.size(); nIdx++) {
						BYTE byData = cmdList[nIdx].toInt(&bOK, 16) & 0xff;
						if (bOK) {
							vtData.push_back(byData);
						}
						else {
							qout << qtr("%1 不是合法的16进制数值").arg(cmdList[nIdx]) << endl;
							vtData.clear();
							break;
						}
					}

					if (vtData.size() > 0)
						editMemoryInfo(hProcess, (LPVOID)dwAddr, vtData);

				}
				else {
					qout << qtr("输入的地址不合法") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("a")) { //a:写入汇编指令
			QString strAsmCodeLine;
			if (cmdList.size() < 2) {
				qout << QString::asprintf("%08X: ", pExceptionAddr) << flush;
				strAsmCodeLine = qin.readLine();
				writeAsmCode2Memory(hProcess, pExceptionAddr, strAsmCodeLine);
			}
			else {
				bool bOK = true;
				DWORD dwAddr = cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					qout << QString::asprintf("%08X: ", dwAddr) << flush;
					strAsmCodeLine = qin.readLine();
					writeAsmCode2Memory(hProcess, (LPVOID)dwAddr, strAsmCodeLine);
				}
				else {
					qout << qtr("输入的地址不合法") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("g")) { //g:继续运行
			m_bGo = true;
			setBreakpointTF(hThread);
			m_bUserTF = true;
			break;
		}
	}
}

//如果指定地址处的指令是CALL，返回CALL指令的长度
//否则返回0
//判断的方法参考了《CALL指令有多少种写法》一文
//http://blog.ftofficer.com/2010/04/n-forms-of-call-instructions
int CDebuggerMain::isCallInstruction(DWORD dwAddr)
{
	BYTE instruct[20] = {};
	DWORD dwNumOfByteRead = 0;
	if (!ReadProcessMemory(m_hProcess, (LPVOID)dwAddr, instruct, 20, &dwNumOfByteRead)) {
		DBGPRINT("读取进程内存失败");
		return 0;
	}

	switch (instruct[0])
	{
	case 0xE8:
		return 5;
	case 0x9A:
		return 7;
	case 0xFF:
		switch (instruct[1])
		{
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x16:
		case 0x17:
		case 0xD0:
		case 0xD1:
		case 0xD2:
		case 0xD3:
		case 0xD4:
		case 0xD5:
		case 0xD6:
		case 0xD7:
			return 2;
		case 0x14:
		case 0x50:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
			return 3;
		case 0x15:
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x95:
		case 0x96:
		case 0x97:
			return 6;
		case 0x94:
			return 7;
		}
	default:
		return 0;
	}
}

void CDebuggerMain::setDebuggeeContext(PCONTEXT pContext)
{
	pContext->ContextFlags = CONTEXT_FULL;
	if (!SetThreadContext(m_hThread, pContext))
		DBGPRINT("设置线程上下文失败");
}

void CDebuggerMain::getDebuggeeContext(PCONTEXT pContext)
{
	pContext->ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(m_hThread, pContext))
		DBGPRINT("获取线程上下文失败");
}

void CDebuggerMain::getNextInstructAddr(HANDLE hProcess, LPVOID pAddr, LPVOID *pNextAddr)
{
	LPBYTE opcode[200] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProcess, pAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("读取内存失败");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pAddr;
		disAsm.Archi = 0;  //x86汇编
		int nLen = 0;

		//nLen返回的是反汇编出来的指令的机器码字节数
		// 如果反汇编失败，则返回-1
		nLen = Disasm(&disAsm);
		if (nLen == -1) {
			pNextAddr = nullptr;
			DBGPRINT("反汇编失败");
		}
		else {
			*pNextAddr = (LPVOID)(disAsm.VirtualAddr + nLen);
		}
	}
}

void CDebuggerMain::editRegister(HANDLE hThread, QString strRegName, DWORD dwVal)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("获取线程上下文失败");
	
	int nIdx = 0;
	if ((nIdx = m_vtRegName.indexOf(strRegName.toUpper())) != -1) {
		QString strTmp = m_vtRegName.at(nIdx);
		if (strTmp == qtr("EAX")) {
			ct.Eax = dwVal;
		}
		else if (strTmp == qtr("ECX")) {
			ct.Ecx = dwVal;
		}
		else if (strTmp == qtr("EDX")) {
			ct.Edx = dwVal;
		}
		else if (strTmp == qtr("EBX")) {
			ct.Ebx = dwVal;
		}
		else if (strTmp == qtr("EBP")) {
			ct.Ebp = dwVal;
		}
		else if (strTmp == qtr("ESP")) {
			ct.Esp = dwVal;
		}
		else if (strTmp == qtr("EDI")) {
			ct.Edi = dwVal;
		}
		else if (strTmp == qtr("ESI")) {
			ct.Esi = dwVal;
		}
		else if (strTmp == qtr("EIP")) {
			ct.Eip = dwVal;
		}

		if (!SetThreadContext(hThread, &ct))
			DBGPRINT("设置线程上下文失败");
	}
	else {
		qout << qtr("%1 不存在该寄存器").arg(strRegName) << endl;
	}
}

void CDebuggerMain::writeAsmCode2Memory(HANDLE hProc, LPVOID pAddr, QString strAsmCode)
{
	ks_engine *pAsmEngine = nullptr;
	if (ks_open(KS_ARCH_X86, KS_MODE_32, &pAsmEngine) == KS_ERR_OK) {
		BYTE *pOpCode = nullptr;
		size_t nOpCodeBytes = 0;
		size_t stat_count = 0;

		QByteArray bArr = strAsmCode.toUtf8();
		const char *asmCode = bArr.data();
		int nRet = ks_asm(pAsmEngine,
			asmCode,
			(uint64_t)pAddr,
			&pOpCode,
			&nOpCodeBytes,
			&stat_count);

		int a = 2;
		if (nRet == -1) { //汇编失败
		}
		else {
			//将指令写入内存
			SIZE_T nBytesWrite = 0;
			WriteProcessMemory(hProc, pAddr, pOpCode, nOpCodeBytes, &nBytesWrite);
			ks_free(pOpCode);
			ks_close(pAsmEngine);
		}
	}
}

void CDebuggerMain::editMemoryInfo(HANDLE hProc, LPVOID pAddr, QVector<BYTE> &vtData)
{
	DWORD pCurAddr = (DWORD)pAddr;

	SIZE_T bytesWrite = 0;
	for (int i = 0; i < vtData.size(); i++) {
		WriteProcessMemory(hProc, (LPVOID)pCurAddr, &vtData[i], 1, &bytesWrite);
		pCurAddr++;
	}
}

void CDebuggerMain::showRegisterInfo(HANDLE hThread)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("获取线程上下文失败");

	qout << qtr("寄存器信息:") << endl;
	qout << QString::asprintf("EAX:%08X ECX:%08X EDX:%08X EBX:%08X EBP:%08X ESP:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx, ct.Ebp, ct.Esp) << endl;
	qout << QString::asprintf("ESI:%08X EDI:%08X EIP:%08X", ct.Esi, ct.Edi, ct.Eip) << endl;
}

void CDebuggerMain::showExecuteModuleInfo()
{
	if (m_vtModule.isEmpty())
		traverseExecModule(m_dwDebuggeePID);

	qout << qtr("可执行模块: ") << endl;
	qout << qtr("基址┃大小┃路径") << endl;
	for (auto &execMod : m_vtModule) {
		qout << QString::asprintf("%08X┃%08X┃", 
			execMod.dwModBaseAddr, execMod.dwModSize) << execMod.strModPath << endl;
	}
}

void CDebuggerMain::showStackInfo(HANDLE hProc, HANDLE hThread, int nCnt)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("获取线程上下文失败");

	DWORD *pBuf = new DWORD[nCnt]{};
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, (LPCVOID)ct.Esp, pBuf, nCnt*sizeof(DWORD), &bytesRead)) {
		DBGPRINT("读取进程内存失败");
	}
	else {
		qout << qtr("栈数据: \n");
		for (int i = 0; i < nCnt; i++) {
			qout << QString::asprintf("%08X┃%08X",
				ct.Esp + i * 4, pBuf[i]) << endl;
		}
	}

	delete[] pBuf; pBuf = nullptr;
}

void CDebuggerMain::showMemoryInfo(HANDLE hProc, LPVOID pAddr, int nNumOfBytesRead)
{
	SIZE_T byteRead = 0;
	BYTE data;
	QString strAsciiLine;
	int nExtraBytes = nNumOfBytesRead % 16;

	DWORD pCurAddr = (DWORD)pAddr;

	for (int i = 0; i < nNumOfBytesRead; i++) {
		if (strAsciiLine.size() == 0) {
			qout << QString::asprintf("%08X┃", pCurAddr) << flush;
		}

		if (ReadProcessMemory(hProc, (LPVOID)pCurAddr, &data, 1, &byteRead)) {
			if ((strAsciiLine.size() + 1) % 4 == 0)
				qout << QString::asprintf("%02X┃", data) << flush;
			else
				qout << QString::asprintf("%02X ", data) << flush;
			if (data >= 0x00 && data <= 0x1F)
				strAsciiLine.append(".");
			else if (data >= 0x81 && data <= 0xFF)
				strAsciiLine.append("?");
			else
				strAsciiLine.append((char)data);
		}
		else {  //读取失败
			qout << qtr("?? ");
			strAsciiLine.append(".");
		}

		//如果一行满 16 个字节,则输出ASCII字符,并输出换行
		if (strAsciiLine.size() == 16) {
			qout << strAsciiLine << endl;
			strAsciiLine.clear();
		}

		pCurAddr++;
	}

	if (nExtraBytes > 0) {
		for (int i = 0; i < 16 - nExtraBytes; i++) {
			if ((i+1+nExtraBytes) % 4 == 0)
				qout << qtr("**┃") << flush;
			else
				qout << qtr("** ") << flush;
		}
		qout << strAsciiLine << endl;
	}
}

void CDebuggerMain::showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr)
{
	//输出寄存器信息
	showRegisterInfo(hThread);

	//输出栈信息
	showStackInfo(hProc, hThread, 10);

	//输出反汇编信息
	showDisambleInfo(hProc, pExceptionAddr, 10);
}

void CDebuggerMain::showDisambleInfo(HANDLE hProc, LPVOID pAddr, int nCnt)
{
	BYTE opcode[200] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, pAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("读取内存失败");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pAddr;
		disAsm.Archi = 0;  //x86汇编
		int nLen = 0;

		//nLen返回的是反汇编出来的指令的机器码字节数
		// 如果反汇编失败，则返回-1
		qout << qtr("反汇编信息: \n");
		int nSize = 0;
		while (nSize < nCnt) {
			nLen = Disasm(&disAsm);
			if (nLen == -1)
				break;

			//输出反汇编
			qout << QString::asprintf("0x%08X┃%s",
				(DWORD)disAsm.VirtualAddr, disAsm.CompleteInstr) << endl;

			disAsm.EIP += nLen;
			disAsm.VirtualAddr += nLen;
			++nSize;
		}
	}
}

void CDebuggerMain::resetAllBreakpoint(HANDLE hProcess)
{
	BREAKPOINT tBp = {};
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			setBreakpointCC(hProcess, bp.pAddr, &tBp);
		}
		else if (bp.dwType == EXCEPTION_SINGLE_STEP) {

		}
	}
}

void CDebuggerMain::clearAllBreakpoint(HANDLE hProcess)
{
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			DWORD dwRead = 0;
			if (!WriteProcessMemory(hProcess, bp.pAddr, &bp.oldData, 1, &dwRead)) {
				DBGPRINT("写入进程内存失败");
				return;
			}
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
	ct.ContextFlags = CONTEXT_FULL;
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

	//删除所有断点
	clearAllBreakpoint(m_hProcess);
}

//设置硬件执行断点
bool CDebuggerMain::setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr)
{
	/*
	  设置硬件执行断点时，长度只能为1(LEN0~LEN3设置为0时表示长度为1)	
	*/
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;

	if (pDr7->L0 == 0) { //DR0没有被使用
		ct.Dr0 = pAddr;
		pDr7->RW0 = 0;
		pDr7->LEN0 = 0;
		pDr7->L0 = 1;
	}
	else if (pDr7->L1 == 0) { //DR1没有被使用
		ct.Dr1 = pAddr;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
		pDr7->L1 = 1;
	}
	else if (pDr7->L2 == 0) { //DR2没有被使用
		ct.Dr2 = pAddr;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
		pDr7->L2 = 1;
	}
	else if (pDr7->L3 == 0) { //DR3没有被使用
		ct.Dr3 = pAddr;
		pDr7->RW3 = 0;
		pDr7->LEN3 = 0;
		pDr7->L3 = 1;
	}
	else {
		return false;
	}

	SetThreadContext(hThread, &ct);
	return true;
}

bool CDebuggerMain::setBreakpointHardRW(HANDLE hThread, ULONG_PTR pAddr, HardBreakpointType type, DWORD dwLen)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);

	//对地址和长度进行对齐处理(向上取整)
	if (dwLen == 1) { //2字节的对齐粒度
		pAddr = pAddr - pAddr % 2;
	}
	else if (dwLen == 3) { //4字节的对齐粒度
		pAddr = pAddr - pAddr % 4;
	}
	else if (dwLen > 3) {
		return false;
	}

	//判断哪些寄存器没有被使用
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;
	if (pDr7->L0 == 0) { //DR0 没有被使用
		ct.Dr0 = pAddr;
		pDr7->RW0 = type;
		pDr7->LEN0 = dwLen;
	}
	else if (pDr7->L1 == 0) { //DR1 没有被使用
		ct.Dr1 = pAddr;
		pDr7->RW1 = type;
		pDr7->LEN1 = dwLen;
	}
	else if (pDr7->L2 == 0) { //DR2 没有被使用
		ct.Dr2 = pAddr;
		pDr7->RW2 = type;
		pDr7->LEN2 = dwLen;
	}
	else if (pDr7->L3 == 0) { //DR3 没有被使用
		ct.Dr3 = pAddr;
		pDr7->RW3 = type;
		pDr7->LEN3 = dwLen;
	}
	else {
		return false;
	}

	SetThreadContext(hThread, &ct);
	return true;
}

bool CDebuggerMain::isHardBreakpoint(HANDLE hThread)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);

	DBG_REG6 *pDr6 = (DBG_REG6*)&ct.Dr6;
	if (pDr6->B0 || pDr6->B1 || pDr6->B2 || pDr6->B3)
		return true;

	return false;
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

