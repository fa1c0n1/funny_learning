#include "DebuggerMain.h"
#include "stdafx.h"
#include "Comm.h"
#include "RegStruct.h"
#include "PeParser.h"
#include <QFile>
#include <QDataStream>
#include <TlHelp32.h>
#include <DbgHelp.h>

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"
#include "keystone/keystone.h"

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib,"keystone/x86/keystone_x86.lib")
#pragma comment(lib, "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")

CDebuggerMain::CDebuggerMain()
	: m_bSystemBreakpoint(true), m_bUserTF(true),
	m_bGo(false), m_bTmpCC(false), m_bAttach(false), m_bHaveBpMem(false)
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
_BEGIN:
	showMainMenu();

	QString strChoice;
	qout << qtr("请选择(1/2): ") << flush;
	strChoice = qin.readLine();
	QStringList choiceCmdList = strChoice.split(' ', QString::SkipEmptyParts);

	if (choiceCmdList.size() < 1) {
		system("cls");
		goto _BEGIN;
	}
	else {
		if (choiceCmdList[0] == qtr("1")) {

		}
		else if (choiceCmdList[0] == qtr("2")) {
			showAll32Process();
		}
		else {
			system("cls");
			goto _BEGIN;
		}
	}
	
	startDebug(choiceCmdList[0]);
}

//显示主菜单
void CDebuggerMain::showMainMenu()
{
	qout << qtr(" ━━━━━━ ") << endl;
	qout << qtr("┃1.打开文件┃") << endl;
	qout << qtr("┃2.附加进程┃") << endl;
	qout << qtr(" ━━━━━━ ") << endl;
}

//以打开文件的方式打开进程
bool CDebuggerMain::openProc(QString strFile)
{
	if (strFile.isEmpty())
		return false;

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
		DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&stcStartupInfo,
		&stcProcInfo);

	m_dwDebuggeePID = stcProcInfo.dwProcessId;
	m_hProcess = stcProcInfo.hProcess;
	m_hThread = stcProcInfo.hThread;

	return bRet;
}

//附加指定进程
bool CDebuggerMain::openProc(DWORD dwPID)
{
	getSeDebugPrivilge();
	return DebugActiveProcess(dwPID);
}

//调试主循环
void CDebuggerMain::startDebug(QString strChoice)
{
	if (strChoice == qtr("1")) {
		qout << qtr("请输入文件路径: ") << flush;
		m_strFile = qin.readLine();
		if (!openProc(m_strFile)) {
			qout << qtr("创建调试进程失败") << endl;
			return;
		}
	}
	else if (strChoice == qtr("2")) {
		QString strTmp;
		qout << qtr("请输入进程ID: ") << flush;
		strTmp = qin.readLine();
		bool bOK = true;
		m_dwDebuggeePID = strTmp.toULong(&bOK, 10);
		if (bOK) {
			if (!openProc(m_dwDebuggeePID)) {
				qout << qtr("附加进程失败") << endl;
				return;
			}
			else {
				m_bAttach = true;
			}
		}
		else {
			qout << qtr("输入错误") << endl;
			return;
		}
	}
	else {
		qout << qtr("程序退出") << endl;
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
			if (!m_bAttach) {
				//如果不是附加进程，则在入口点地址处设置软件断点
				CREATE_PROCESS_DEBUG_INFO debugInfo = debugEvent.u.CreateProcessInfo;
				BREAKPOINT bp = {};
				setBreakpointCC(debugInfo.hProcess,
					debugEvent.u.CreateProcessInfo.lpStartAddress, &bp);
				m_listBp.push_back(bp);

				//初始化符号处理器
				if (SymInitialize(m_hProcess, NULL, FALSE)) {
					//加载模块的调试信息
					DWORD64 moduleAddr = SymLoadModule64(
						m_hProcess,
						debugInfo.hFile,
						NULL,
						NULL,
						(DWORD64)debugInfo.lpBaseOfImage,
						0);

					if (moduleAddr == 0) {
						DBGPRINT("SymLoadModule64 failed");
					}
				}
				else {
					DBGPRINT("初始化符号处理器失败");
				}

				CloseHandle(debugInfo.hFile);
				CloseHandle(debugInfo.hProcess);
				CloseHandle(debugInfo.hThread);
			}
		}
			break;
		case EXCEPTION_DEBUG_EVENT:
			dwRet = onException(&debugEvent);
			break;
		case EXIT_PROCESS_DEBUG_EVENT:
			onProcessExited(&debugEvent.u.ExitProcess);
			break;
		case LOAD_DLL_DEBUG_EVENT:
			onDllLoaded(&debugEvent.u.LoadDll);
			break;
		case UNLOAD_DLL_DEBUG_EVENT:
			onDllUnloaded(&debugEvent.u.UnloadDll);
			break;
		default:
			break;
		}

		//回复调试事件的处理结果
		ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwRet);
	}
}

//反反调试
void CDebuggerMain::antiAntiDebugPEB(HANDLE hProcess)
{
	DWORD dwPEBAddr = *(DWORD*)(0x7EFDD000 + 0x30);
	BYTE *pPEBAddr = (BYTE*)dwPEBAddr + 2;
	BYTE bNum = 0;
	DWORD dwByteWrite = 0;
	WriteProcessMemory(hProcess, (LPVOID)pPEBAddr, (LPCVOID)&bNum, 1, &dwByteWrite);
}

//遍历可执行模块
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

//响应异常事件
DWORD CDebuggerMain::onException(DEBUG_EVENT *pEvent)
{
	DWORD dwRet = DBG_CONTINUE;
	EXCEPTION_RECORD &er = pEvent->u.Exception.ExceptionRecord;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEvent->dwProcessId);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pEvent->dwThreadId);

	//被调试进程产生的第一个异常事件，这个
	//  异常事件就是系统断点
	if (!m_bHaveBpMem) {
		qout << QString::asprintf("\t异常代码: %08X", er.ExceptionCode) << endl;
		qout << QString::asprintf("\t异常地址: %08X", er.ExceptionAddress) << endl;
	}

	if (m_bSystemBreakpoint) {
		antiAntiDebugPEB(hProcess);
		qout << qtr("到达系统断点,忽略") << endl;
		m_bSystemBreakpoint = false;
		
		if (m_bAttach)
			userInput(hProcess, hThread, er.ExceptionAddress);	

		return DBG_CONTINUE;
	}	

	switch (er.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT: //软件断点
	{
		qout << qtr("触发软件断点") << endl;
		
		clearAllBreakpoint(hProcess, hThread);

		//判断是否为条件断点
		for (auto &bp : m_listBp) {
			if ((DWORD)bp.pAddr == (DWORD)er.ExceptionAddress) {
				if (!bp.bCondition) { //不是条件断点
					break;
				}
				else { //是条件断点,则判断是否符合条件
					CONTEXT tmpCt = {};
					getDebuggeeContext(&tmpCt, hThread);
					if (isTriggeredBreakpointCond(&tmpCt, bp)) {//符合条件
						break;
					}
					else {
						tmpCt.Eip--;
						setDebuggeeContext(&tmpCt, hThread);
						setBreakpointTF(hProcess, hThread);
						goto _EXIT;
					}
				}
			}
		}

		//输出调试信息
		showDebugInfo(hProcess, hThread, er.ExceptionAddress);

		//如果是单步步过设置的临时断点，则从集合中删除该断点
		if (m_bTmpCC)
			m_listBp.pop_back();

		CONTEXT ct = {};
		getDebuggeeContext(&ct, hThread);
		ct.Eip--;
		setDebuggeeContext(&ct, hThread);
	}
		break;
	case EXCEPTION_ACCESS_VIOLATION:  //内存访问异常(内存访问断点)
	{
		m_bHaveBpMem = false;
		clearAllBreakpoint(hProcess, hThread);
		if (!m_listBpMem.isEmpty()) {
			BREAKPOINTMEM bpm = m_listBpMem.at(0);
			if (bpm.pAddr == er.ExceptionAddress) {
				qout << qtr("触发内存断点") << endl;
				showDebugInfo(hProcess, hThread, er.ExceptionAddress);
				break;
			}
			else {
				setBreakpointTF(hProcess, hThread);
				m_bHaveBpMem = true;
				if (m_bGo)
					goto _EXIT;
				//else
			}
		}
	}

		break;

	//TF和硬件断点异常
	//  通过DR6寄存器进一步判断这个异常是
	//  TF引发的还是 DR0~DR3 引发的
	case EXCEPTION_SINGLE_STEP:
		if (!m_bHaveBpMem)
			//输出调试信息
			showDebugInfo(hProcess, hThread, er.ExceptionAddress);

		if (isTriggeredBreakpointHard(hThread)) {
			qout << qtr("触发硬件断点") << endl;
			break;
		}
		
		//将所有的断点都恢复
		resetAllBreakpoint(hProcess, hThread);
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

//响应dll加载事件
void CDebuggerMain::onDllLoaded(LOAD_DLL_DEBUG_INFO *pInfo)
{
	TCHAR szBuf[MAX_PATH] = {};
	GetModuleFileName((HMODULE)pInfo->lpBaseOfDll, szBuf, MAX_PATH);
	QString strModulePath = QString::fromWCharArray(szBuf);
	QByteArray bArr = strModulePath.toUtf8();
	const char *szModulePath = bArr.data();

	//加载模块的调试信息
	DWORD64 moduleAddress = SymLoadModule64(
		m_hProcess,
		pInfo->hFile,
		szModulePath,
		NULL,
		(DWORD64)pInfo->lpBaseOfDll,
		0);

	if (moduleAddress == 0) {
		DBGPRINT("SymLoadModule64 failed");
	}

	CloseHandle(pInfo->hFile);
}

void CDebuggerMain::onDllUnloaded(UNLOAD_DLL_DEBUG_INFO *pInfo)
{
	SymUnloadModule64(m_hProcess, (DWORD64)pInfo->lpBaseOfDll);
}

void CDebuggerMain::onProcessExited(EXIT_PROCESS_DEBUG_INFO *pEvent)
{
	qout << qtr("Debuggee was terminated.") << endl;
}

//接收用户输入
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

			setBreakpointTF(hProcess, hThread);
			//m_bUserTF = true;
			break;
		}
		else if (cmdList[0] == qtr("n")) { //n:单步步过
			//先判断当前EIP指向的指令是否为CALL指令
			CONTEXT tContext;
			getDebuggeeContext(&tContext, hThread);
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
				setBreakpointTF(hProcess, hThread);
				//m_bUserTF = true;
				break;
			}
		}
		else if (cmdList[0] == qtr("b")) {  //b:设置软件断点
			if (cmdList.size() < 2) {
				showAllBreakpointCCInfo();
			}
			else if (cmdList.size() < 3) { //b <addr> 设置软件断点
				bool bOK = true;
				LPVOID pAddr = (LPVOID)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					BREAKPOINT bp = {};
					if (!setBreakpointCC(hProcess, pAddr, &bp)) {
						DBGPRINT("设置软件断点失败");
					}
					else {
						m_listBp.push_back(bp);
					}
				}
				else {
					qout << qtr("%1不是一个合法的16进制值").arg(cmdList[1]) << endl;
				}
			}
			else if (cmdList.size() < 4) { //b -d <addr> 删除软件断点
				if (cmdList[1] == qtr("-d")) {
					bool bOK = true;
					LPVOID pAddr = (LPVOID)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!rmBreakpointCC(hProcess, pAddr))
							DBGPRINT("删除软件断点失败");
					}
					else {
						qout << qtr("%1不是一个合法的16进制值").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("参数错误") << endl;
				}
			}
			else if (cmdList.size() < 5) { //b -c <条件> <addr> 设置条件断点
				if (cmdList[1] == qtr("-c")) {
					QStringList condList = cmdList[2].split("==", QString::SkipEmptyParts);
					if (condList.size() == 2) {
						if (m_vtRegName.contains(condList[0].toUpper())) {
							bool bOK = true;
							DWORD dwCondVal = condList[1].toULong(&bOK, 16);
							if (bOK) {
								BREAKPOINT tmpBp = {};
								tmpBp.bCondition = true;
								tmpBp.strCondRegName = condList[0].toUpper();
								tmpBp.dwCondVal = dwCondVal;

								DWORD dwAddr = cmdList[3].toULong(&bOK, 16);
								if (bOK) {
									if (!setBreakpointCC(hProcess, (LPVOID)dwAddr, &tmpBp)) {
										DBGPRINT("设置软件断点失败");
									}
									else {
										m_listBp.push_back(tmpBp);
									}
								}
								else {
									qout << QString::asprintf("%1不是一个合法的16进制值").arg(cmdList[3]) << endl;
								}
							}
							else {
								qout << QString::asprintf("%1不是一个合法的16进制值").arg(condList[1]) << endl;
							}
						}
						else {
							qout << qtr("不支持该寄存器") << endl;
						}
					}
					else {
						qout << qtr("条件表达式错误:[例: eax==5](只能用16进制数)") << endl;
					}
				}
				else {
					qout << qtr("参数错误") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bh")) { //bh:设置硬件断点
			BREAKPOINTHARD bph = {};
			if (cmdList.size() < 2) {
				showAllBreakpointHardInfo();
			}
			else if (cmdList.size() < 3) {
				qout << qtr("缺少参数") << endl;
			}
			else if (cmdList.size() < 4) {  
				if (cmdList[1] == qtr("-e")) { //bh [-e] <addr> 设置硬件执行断点
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointHardExec(hThread, pAddr, &bph))
							DBGPRINT("设置硬件执行断点失败");
						else
							m_listBpHard.push_back(bph);
					}
					else {
						qout << qtr("%1不是一个合法的16进制值").arg(cmdList[2]) << endl;
					}
				}
				else if (cmdList[1] == qtr("-d")) { //bh [-d] <addr> 删除硬件断点
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (m_listBpHard.isEmpty()) {
							qout << qtr("未设置硬件断点") << endl;
						}
						else {
							if (!rmBreakpointHard(hThread, pAddr))
								DBGPRINT("删除硬件断点失败");
						}
					}
					else {
						qout << qtr("%1不是一个合法的16进制值").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("参数错误") << endl;
				}
				
			}
			else if (cmdList.size() < 5) { //bh [-rw] <addr> <len> 设置硬件读写断点
				if (cmdList[1] == qtr("-rw")) {
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						DWORD dwLen = (DWORD)cmdList[3].toULong(&bOK, 10);
						if (bOK) {
							if (dwLen != 1 && dwLen != 2 && dwLen != 4) {
								qout << qtr("长度只能是1，2或4") << endl;
							}
							else {
								if (!setBreakpointHardRW(hThread, pAddr, HARDBP_READWRITE, dwLen - 1, &bph))
									DBGPRINT("设置硬件读写断点失败");
								else
									m_listBpHard.push_back(bph);
							}
						}
						else {
							qout << qtr("%1不是一个合法的10进制值").arg(cmdList[3]) << endl;
						}
					}
					else {
						qout << qtr("%1不是一个合法的16进制值").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("参数错误") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bm")) { //bm:设置内存断点
			if (cmdList.size() < 2) {
				showAllBreakpointMemInfo();
			}
			else if (cmdList.size() < 3) {
				qout << qtr("缺少参数") << endl;
			}
			else if (cmdList.size() < 4) { 
				BREAKPOINTMEM bpm = {};
				if (cmdList[1] == qtr("-a")) { //bm [-a] <addr> 设置内存访问断点
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointMem(hProcess, pAddr, PAGE_NOACCESS, &bpm))
							DBGPRINT("设置内存访问断点失败");
						else
							m_listBpMem.push_back(bpm);
					}
					else {
						qout << qtr("%1不是一个合法的16进制值") << endl;
					}
				}
				else if (cmdList[1] == qtr("-w")) { //bm [-w] <addr> 设置内存写入断点
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointMem(hProcess, pAddr, PAGE_READONLY, &bpm))
							DBGPRINT("设置内存写入断点失败");
						else
							m_listBpMem.push_back(bpm);
					}
					else {
						qout << qtr("%1不是一个合法的16进制值") << endl;
					}
				}
				else if (cmdList[1] == qtr("-d")) { //bm [-d] <addr> 删除内存断点
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (m_listBpMem.isEmpty()) {
							qout << qtr("未设置内存断点") << endl;
						}
						else {
							if (!rmBreakpointMem(hProcess))
								DBGPRINT("删除内存断点失败");
							else
								m_listBpMem.pop_back();
						}
					}
					else {
						qout << qtr("%1不是一个合法的16进制值") << endl;
					}
				}
				else {
					qout << qtr("参数错误") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("lm")) { //lm:查看可执行模块信息
			if (cmdList.size() < 2) {
				showExecuteModuleInfo();
			}
			else if (cmdList.size() < 3) {
				if (cmdList[1] == qtr("-pe")) { //lm -pe :解析可执行模块
					qout << qtr("请输入要查看的模块的基址: ") << flush;
					QString strBaseAddr;
					strBaseAddr = qin.readLine();

					bool bOK = true;
					DWORD dwBaseAddr = strBaseAddr.toULong(&bOK, 16);
					if (bOK) {
						EXECMODULE execMod = {};
						if (findModuleInCurProcess(dwBaseAddr, &execMod)) {
							CPeParser peParser;
							if (peParser.parsePE(hProcess, dwBaseAddr, execMod.dwModSize)) {
								QString strPeShow;
								qout << qtr("请输入(i:显示导入表，e:显示导出表):") << flush;
								qin >> strPeShow;
								if (strPeShow == qtr("i")) {
									peParser.showImportTableInfo();
								}
								else if (strPeShow == qtr("e")) {
									peParser.showExportTableInfo();
								}
								else {
									qout << qtr("输入错误") << endl;
								}
							}
							else {
								qout << qtr("解析失败") << endl;
							}
						}
						else {
							qout << qtr("被调试进程没有该模块") << endl;
						}
					}
					else {
						qout << qtr("%1不是一个合法的16进制值").arg(strBaseAddr) << endl;
					}
				} else {
					qout << qtr("参数错误") << endl;
				}
			}

		}
		else if (cmdList[0] == qtr("stack")) { //stack:查看栈信息
			if (cmdList.size() < 2) {
				showStackInfo(hProcess, hThread, 10);
			}
			else if (cmdList.size() < 3) {
				bool bOK = true;
				int nCnt = cmdList[1].toInt(&bOK, 10);
				if (bOK) {
					showStackInfo(hProcess, hThread, nCnt);
				}
				else {
					qout << qtr("%1不是一个合法的10进制值").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("dump")) { //dump <start_addr> <end_addr> <file>
			if (cmdList.size() > 3) {
				bool bOK = true;
				ULONG pStartAddr = cmdList[1].toULong(&bOK, 16);
				if (!bOK) {
					qout << qtr("%1不是一个合法的10进制值").arg(cmdList[1]) << endl;
				}
				else {
					ULONG pEndAddr = cmdList[2].toULong(&bOK, 16);
					if (!bOK) {
						qout << qtr("%1不是一个合法的10进制值").arg(cmdList[2]) << endl;
					}
					else {
						if (pStartAddr > pEndAddr) {
							qout << qtr("参数错误:") << endl;
						}
						else {
							dump2file(hProcess, pStartAddr, pEndAddr, cmdList[3]);
						}
					}
				}
			}
			else {
				qout << qtr("缺少参数") << endl;
			}
		}
		else if (cmdList[0] == qtr("find")) {
			if (cmdList.size() > 1) {
				DWORD dwAddr = findApiAddress(hProcess, cmdList[1]);
				if (dwAddr == 0) {
					qout << qtr("未找到该函数的地址") << endl;
				}
				else {
					qout << cmdList[1] << QString::asprintf("的地址为: %08X", dwAddr) << endl;
				}
			}
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
			if (cmdList.size() < 3) {
				qout << qtr("缺少参数") << endl;
			}
			else { //e <addr> [hex1] [hex2] ...
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
		else if (cmdList[0] == qtr("l")) { //l:查看源码
			onLHandler(cmdList, hProcess, hThread);
		}
		else if (cmdList[0] == qtr("g")) { //g:继续运行
			m_bGo = true;
			setBreakpointTF(hProcess, hThread);
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

//设置被调试进程的线程上下文
void CDebuggerMain::setDebuggeeContext(PCONTEXT pContext, HANDLE hThread)
{
	pContext->ContextFlags = CONTEXT_FULL;
	if (!SetThreadContext(hThread, pContext))
		DBGPRINT("设置线程上下文失败");
}

//获取被调试进程的线程上下文
void CDebuggerMain::getDebuggeeContext(PCONTEXT pContext, HANDLE hThread)
{
	pContext->ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, pContext))
		DBGPRINT("获取线程上下文失败");
}

#if 0
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
#endif

//修改寄存器的值
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

//将汇编指令写入内存
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

//修改内存数据
void CDebuggerMain::editMemoryInfo(HANDLE hProc, LPVOID pAddr, QVector<BYTE> &vtData)
{
	DWORD pCurAddr = (DWORD)pAddr;

	SIZE_T bytesWrite = 0;
	for (int i = 0; i < vtData.size(); i++) {
		WriteProcessMemory(hProc, (LPVOID)pCurAddr, &vtData[i], 1, &bytesWrite);
		pCurAddr++;
	}
}

//显示寄存器信息
void CDebuggerMain::showRegisterInfo(HANDLE hThread)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("获取线程上下文失败");

	qout << qtr("寄存器信息:") << endl;
	qout << QString::asprintf("EAX:%08X ECX:%08X EDX:%08X EBX:%08X EBP:%08X ESP:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx, ct.Ebp, ct.Esp) << endl;
	qout << QString::asprintf("ESI:%08X EDI:%08X EIP:%08X", ct.Esi, ct.Edi, ct.Eip) << endl;
	qout << QString::asprintf("FS:%08X", ct.SegFs) << endl;

	
	qout << qtr("调试寄存器信息:") << endl;
	qout << QString::asprintf("Dr0:%08X  Dr1:%08X  Dr2:%08X  Dr3:%08X", 
		ct.Dr0, ct.Dr1, ct.Dr2, ct.Dr3) << endl;

	DBG_REG6 *pDr6 = (DBG_REG6*)&ct.Dr6;
	qout << QString::asprintf("Dr6:[B0=%d B1=%d B2=%d B3=%d BD=%d BS=%d BT=%d]",
		pDr6->B0, pDr6->B1, pDr6->B2, pDr6->B3, pDr6->BD, pDr6->BS, pDr6->BT) << endl;

	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;
	qout << QString::asprintf("Dr7:[L0=%d G0=%d RW0=%d LEN0=%d]", 
		pDr7->L0, pDr7->G0, pDr7->RW0, pDr7->LEN0) << endl;
	qout << QString::asprintf("Dr7:[L1=%d G1=%d RW1=%d LEN1=%d]", 
		pDr7->L1, pDr7->G1, pDr7->RW1, pDr7->LEN1) << endl;
	qout << QString::asprintf("Dr7:[L2=%d G2=%d RW2=%d LEN2=%d]", 
		pDr7->L2, pDr7->G2, pDr7->RW2, pDr7->LEN2) << endl;
	qout << QString::asprintf("Dr7:[L3=%d G3=%d RW3=%d LEN3=%d]", 
		pDr7->L3, pDr7->G3, pDr7->RW3, pDr7->LEN3) << endl;
}

//显示可执行模块的信息
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

//显示栈的信息
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

//显示内存信息
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

//显示部分调试信息
void CDebuggerMain::showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr)
{
	//输出寄存器信息
	showRegisterInfo(hThread);

	//输出栈信息
	showStackInfo(hProc, hThread, 10);

	//输出反汇编信息
	showDisambleInfo(hProc, pExceptionAddr, 10);
}

//显示反汇编
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

//恢复所有断点
void CDebuggerMain::resetAllBreakpoint(HANDLE hProcess, HANDLE hThread)
{
	//恢复所有软件断点
	BREAKPOINT tBp = {};
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			setBreakpointCC(hProcess, bp.pAddr, &tBp);
		}
	}

	//恢复所有硬件断点
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;
	for (auto &bph : m_listBpHard) {
		switch (bph.dwDrID)
		{
		case 0:
			pDr7->L0 = 1;
			break;
		case 1:
			pDr7->L1 = 1;
			break;
		case 2:
			pDr7->L2 = 1;
			break;
		case 3:
			pDr7->L3 = 1;
			break;
		default:
			break;
		}
	}
	SetThreadContext(hThread, &ct);

	//恢复所有内存断点
	if (!resetBreakpointMem(hProcess))
		DBGPRINT("恢复内存断点失败");

}

//清除所有内存断点
void CDebuggerMain::clearAllBreakpoint(HANDLE hProcess, HANDLE hThread)
{
	//删除所有内存断点
	if (!rmBreakpointMem(hProcess))
		DBGPRINT("删除内存断点失败");

	//删除所有软件断点
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			DWORD dwRead = 0;
			if (!WriteProcessMemory(hProcess, bp.pAddr, &bp.oldData, 1, &dwRead)) {
				DBGPRINT("写入进程内存失败");
				return;
			}
		}
	}

	//删除所有硬件断点
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;
	for (auto &bph : m_listBpHard) {
		switch (bph.dwDrID)
		{
		case 0:
			pDr7->L0 = 0;
			break;
		case 1:
			pDr7->L1 = 0;
			break;
		case 2:
			pDr7->L2 = 0;
			break;
		case 3:
			pDr7->L3 = 0;
			break;
		default:
			break;
		}
	}
	SetThreadContext(hThread, &ct);
}

//设置软件断点
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
void CDebuggerMain::setBreakpointTF(HANDLE hProcess, HANDLE hThread)
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
	clearAllBreakpoint(hProcess, hThread);
}

//设置硬件执行断点
bool CDebuggerMain::setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr, BREAKPOINTHARD *bph)
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
		bph->dwDrID = 0;
	}
	else if (pDr7->L1 == 0) { //DR1没有被使用
		ct.Dr1 = pAddr;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
		pDr7->L1 = 1;
		bph->dwDrID = 1;
	}
	else if (pDr7->L2 == 0) { //DR2没有被使用
		ct.Dr2 = pAddr;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
		pDr7->L2 = 1;
		bph->dwDrID = 2;
	}
	else if (pDr7->L3 == 0) { //DR3没有被使用
		ct.Dr3 = pAddr;
		pDr7->RW3 = 0;
		pDr7->LEN3 = 0;
		pDr7->L3 = 1;
		bph->dwDrID = 3;
	}
	else {
		return false;
	}

	bph->pAddr = (LPVOID)pAddr;
	bph->dwType = HARDBP_EXEC;

	SetThreadContext(hThread, &ct);
	return true;
}

bool CDebuggerMain::setBreakpointHardRW(
	HANDLE hThread, ULONG_PTR pAddr, HardBreakpointType type, 
	DWORD dwLen, BREAKPOINTHARD *bph)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
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
		pDr7->L0 = 1;
		bph->dwDrID = 0;
	}
	else if (pDr7->L1 == 0) { //DR1 没有被使用
		ct.Dr1 = pAddr;
		pDr7->RW1 = type;
		pDr7->LEN1 = dwLen;
		pDr7->L1 = 1;
		bph->dwDrID = 1;
	}
	else if (pDr7->L2 == 0) { //DR2 没有被使用
		ct.Dr2 = pAddr;
		pDr7->RW2 = type;
		pDr7->LEN2 = dwLen;
		pDr7->L2 = 1;
		bph->dwDrID = 2;
	}
	else if (pDr7->L3 == 0) { //DR3 没有被使用
		ct.Dr3 = pAddr;
		pDr7->RW3 = type;
		pDr7->LEN3 = dwLen;
		pDr7->L3 = 1;
		bph->dwDrID = 3;
	}
	else {
		return false;
	}

	bph->dwType = type;
	bph->pAddr = (LPVOID)pAddr;

	SetThreadContext(hThread, &ct);
	return true;
}

//删除硬件断点
bool CDebuggerMain::rmBreakpointHard(HANDLE hThread, ULONG_PTR pAddr)
{
	bool bBpExist = false;
	int nIdx = -1;
	for (auto &bph : m_listBpHard) {
		nIdx++;
		if ((ULONG_PTR)bph.pAddr == pAddr) {
			bBpExist = true;
			break;
		}
	}

	if (!bBpExist) {
		qout << qtr("未在该地址处设置硬件断点") << endl;
		return false;
	}

	BREAKPOINTHARD bph = m_listBpHard.takeAt(nIdx);

	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;

	switch (bph.dwDrID)
	{
	case 0:
	{
		ct.Dr0 = 0;
		pDr7->L0 = 0;
		pDr7->RW0 = 0;
		pDr7->LEN0 = 0;
	}
		break;
	case 1:
	{
		ct.Dr1 = 0;
		pDr7->L1 = 0;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
	}
		break;
	case 2:
	{
		ct.Dr2 = 0;
		pDr7->L2 = 0;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
	}
		break;
	case 3:
	{
		ct.Dr3 = 0;
		pDr7->L3 = 0;
		pDr7->RW3 = 0;
		pDr7->LEN3 = 0;
	}
		break;
	default:
		break;
	}

	SetThreadContext(hThread, &ct);
	return true;
}

//设置内存断点
bool CDebuggerMain::setBreakpointMem(HANDLE hProcess, ULONG_PTR pAddr, INT nType, BREAKPOINTMEM *bpm)
{
	if (!m_listBpMem.isEmpty()) {
		rmBreakpointMem(hProcess);
		m_listBpMem.clear();
	}

	bpm->pPageStartAddr = (LPVOID)(pAddr & 0xFFFFF000);
	bpm->pPageEndAddr = (LPVOID)((DWORD)bpm->pPageStartAddr + 0x1000 - 1);
	bpm->dwNewType = nType;
	bpm->pAddr = (LPVOID)pAddr;
	if (!VirtualProtectEx(hProcess, (LPVOID)pAddr, 1, nType, &bpm->dwOldType))
		return false;

	return true;
}

bool CDebuggerMain::rmBreakpointMem(HANDLE hProcess)
{
	if (m_listBpMem.isEmpty())
		return true;

	BREAKPOINTMEM bpm = m_listBpMem.at(0);
	DWORD dwTmp = 0;
	if (!VirtualProtectEx(hProcess, (LPVOID)bpm.pAddr, 1, bpm.dwOldType, &dwTmp))
		return false;
	return true;
}

bool CDebuggerMain::resetBreakpointMem(HANDLE hProcess)
{
	if (m_listBpMem.isEmpty())
		return true;

	BREAKPOINTMEM bpm = m_listBpMem.at(0);
	DWORD dwTmp = 0;
	if (!VirtualProtectEx(hProcess, (LPVOID)bpm.pAddr, 1, bpm.dwNewType, &dwTmp))
		return false;
	return true;
}

//是否触发了硬件断点
bool CDebuggerMain::isTriggeredBreakpointHard(HANDLE hThread)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);

	DBG_REG6 *pDr6 = (DBG_REG6*)&ct.Dr6;
	if (pDr6->B0 || pDr6->B1 || pDr6->B2 || pDr6->B3)
		return true;

	return false;
}

//是否触发了条件断点
bool CDebuggerMain::isTriggeredBreakpointCond(CONTEXT *pContext, BREAKPOINT &bp)
{
	if (bp.bCondition) {
		if (bp.strCondRegName == qtr("EAX"))
			return bp.dwCondVal == pContext->Eax;
		else if (bp.strCondRegName == qtr("EBX"))
			return bp.dwCondVal == pContext->Ebx;
		else if (bp.strCondRegName == qtr("ECX"))
			return bp.dwCondVal == pContext->Ecx;
		else if (bp.strCondRegName == qtr("EDX"))
			return bp.dwCondVal == pContext->Edx;
		else if (bp.strCondRegName == qtr("ESI"))
			return bp.dwCondVal == pContext->Esi;
		else if (bp.strCondRegName == qtr("EDI"))
			return bp.dwCondVal == pContext->Edi;
		else if (bp.strCondRegName == qtr("EBP"))
			return bp.dwCondVal == pContext->Ebp;
		else if (bp.strCondRegName == qtr("ESP"))
			return bp.dwCondVal == pContext->Esp;
		else if (bp.strCondRegName == qtr("EIP"))
			return false;
	}
	else {
		return false;
	}
}

DWORD CDebuggerMain::findApiAddress(HANDLE hProcess, QString strName)
{
	BYTE buf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)]{};
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buf;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;
	
	TCHAR szName[MAX_SYM_NAME] = {};
	strName.toWCharArray(szName);

	//根据名字查询符号信息,输出到 pSymbol 中
	if (!SymFromName(m_hProcess, szName, pSymbol))
		return 0;

	return (DWORD)pSymbol->Address;   //返回函数地址
}

//删除软件断点
bool CDebuggerMain::rmBreakpointCC(HANDLE hProc, LPVOID pAddr)
{
	bool bRet = true;
	bool bBpExist = false;
	SIZE_T bytesWrite = 0;
	int i = -1;
	for (auto &bp : m_listBp) {
		i++;
		if (bp.pAddr == pAddr) {
			bBpExist = true;
			if (!WriteProcessMemory(hProc, pAddr, &bp.oldData, 1, &bytesWrite)) {
				DBGPRINT("写入内存失败");
				bRet = false;
				break;
			}
		}
	}

	if (!bBpExist) {
		qout << qtr("该地址处不存在软件断点") << endl;
		return false;
	}

	if (bBpExist && bRet)
		m_listBp.removeAt(i);
	
	return bRet;
}

//显示所有软件断点的信息
void CDebuggerMain::showAllBreakpointCCInfo()
{
	if (m_listBp.isEmpty()) {
		qout << qtr("未设置软件断点") << endl;
		return;
	}

	qout << qtr("软件断点信息:") << endl;
	int i = 0;
	for (auto &bp : m_listBp) {
		qout << QString::asprintf("[%d]: 地址=%08X", i, bp.pAddr) << endl;
		i++;
	}
}

//显示所有硬件断点的信息
void CDebuggerMain::showAllBreakpointHardInfo()
{
	if (m_listBpHard.isEmpty()) {
		qout << qtr("未设置硬件断点") << endl;
		return;
	}

	qout << qtr("硬件断点信息:") << endl;
	int i = 0;
	for (auto &bph : m_listBpHard) {
		qout << QString::asprintf("[%d]: 地址=%08X ", i, bph.pAddr) << flush;
		QString strType;
		if (bph.dwType == HARDBP_EXEC)
			strType = qtr("硬件执行");
		else if (bph.dwType == HARDBP_WRITE)
			strType = qtr("硬件写入");
		else if (bph.dwType == HARDBP_READWRITE)
			strType = qtr("硬件读写");
		else
			strType = qtr("未知");

		qout << qtr("类型=%1 ").arg(strType) << flush;

		QString strDrID;
		if (bph.dwDrID == 0)
			strDrID = qtr("Dr0");
		else if (bph.dwDrID == 1)
			strDrID = qtr("Dr1");
		else if (bph.dwDrID == 2)
			strDrID = qtr("Dr2");
		else if (bph.dwDrID == 3)
			strDrID = qtr("Dr3");
		else
			strDrID = qtr("未知");

		qout << qtr("Dr寄存器=%1").arg(strDrID) << endl;
		i++;
	}
}

//显示所有内存断点的信息
void CDebuggerMain::showAllBreakpointMemInfo()
{
	if (m_listBpMem.isEmpty()) {
		qout << qtr("未设置内存断点") << endl;
		return;
	}

	qout << qtr("内存断点信息:") << endl;

	int i = 0;
	for (auto &bpm : m_listBpMem) {
		qout << QString::asprintf("[%d]: 地址=%08X 所在内存页:[%08X~%08X] ", 
			i, bpm.pAddr, bpm.pPageStartAddr, bpm.pPageEndAddr) << flush;

		QString strType;
		if (bpm.dwNewType == PAGE_NOACCESS)
			strType = qtr("内存访问");
		else if (bpm.dwNewType == PAGE_READONLY)
			strType = qtr("内存写入");
		else
			strType = qtr("未知");

		qout << qtr("类型=%1").arg(strType) << endl;

		i++;
	}
}

//处理 l 命令: 显示源码
void CDebuggerMain::onLHandler(QStringList cmdList, HANDLE hProcess, HANDLE hThread)
{
	int afterLines = 10;
	int beforeLines = 10;

	//获取EIP
	CONTEXT ct;
	getDebuggeeContext(&ct, hThread);

	//获取源文件以及行信息
	IMAGEHLP_LINE64 lineInfo = {};
	lineInfo.SizeOfStruct = sizeof(lineInfo);
	DWORD dwDispacement = 0;

	if (!SymGetLineFromAddr64(m_hProcess, ct.Eip, &dwDispacement, &lineInfo)) {
		DWORD errCode = GetLastError();
		switch (errCode)
		{
		case 126:
			qout << qtr("Debug info in current module has not loaded") << endl;
			break;
		case 487:
			qout << qtr("No debug info in current module.") << endl;
			break;
		default:
			qout << QString::asprintf("errCode=%d, SymGetLineFromAddr64 failed", errCode) << endl;
			break;
		}
		return;
	}

	QString strSrcFile = QString::fromWCharArray(lineInfo.FileName);

	displaySourceLines(
		hProcess,
		strSrcFile,
		lineInfo.LineNumber,
		(ULONG64)lineInfo.Address,
		afterLines, 
		beforeLines);
}

//显示源文件中指定的行
void CDebuggerMain::displaySourceLines(HANDLE hProcess, QString strSrcFile, 
	int nLineNum, ULONG64 ulAddr, int nAfter, int nBefore)
{
	qout << endl;

	QFile tFile(strSrcFile);
	tFile.open(QIODevice::ReadOnly);
	QTextStream inStream(&tFile);

	int nCurLineNum = 1;

	//计算从第几行开始输出
	int nStartLineNum = nLineNum - nBefore;
	if (nStartLineNum < 1)
		nStartLineNum = 1;

	QString strLine;

	//跳过不需要显示的行
	while (nCurLineNum < nStartLineNum) {
		strLine = inStream.readLine();
		++nCurLineNum;
	}

	//输出开始行到当前行之间的行
	while (nCurLineNum < nLineNum) {
		strLine = inStream.readLine();
		displayLine(hProcess, strSrcFile, strLine, nCurLineNum, false);
		++nCurLineNum;
	}

	//输出当前行
	strLine = inStream.readLine();
	displayLine(hProcess, strSrcFile, strLine, nCurLineNum, true);
	++nCurLineNum;

	//输出当前行到最后一行之间的行
	int nLastLineNum = nLineNum + nAfter;
	while (nCurLineNum <= nLastLineNum) {
		if (inStream.atEnd())
			break;

		strLine = inStream.readLine();
		displayLine(hProcess, strSrcFile, strLine, nCurLineNum, false);
		++nCurLineNum;
	}

	tFile.close();
}

//显示源文件中的一行
void CDebuggerMain::displayLine(HANDLE hProcess, QString strSrcFile, 
	QString strLine, int nLineNum, bool bCurLine)
{
	if (bCurLine) {
		qout << qtr("=>") << flush;
	}
	else {
		qout << qtr("  ") << flush;
	}

	LONG lDisplacement;
	IMAGEHLP_LINE64 lineInfo = {};
	lineInfo.SizeOfStruct = sizeof(lineInfo);

	TCHAR szFile[MAX_PATH] = {};
	strSrcFile.toWCharArray(szFile);
	if (!SymGetLineFromName64(m_hProcess, NULL, szFile, nLineNum, &lDisplacement, &lineInfo)) {
		DBGPRINT("SymGetLineFromName64 failed");
		return;
	}

	qout << QString::asprintf("%4d  ", nLineNum) << flush;
	if (lDisplacement == 0) {
		qout << QString::asprintf("%08X  ", lineInfo.Address) << flush;
	}
	else {
		qout << qtr("          ") << flush;
	}

	qout << strLine << endl;
}

//显示所有的32位进程
void CDebuggerMain::showAll32Process()
{
	getAll32Process();
	qout << qtr("进程信息(仅32位进程): 进程ID ┃ 进程名") << endl;

	for (auto &proc : m_vtProcess) {
		qout << QString::asprintf("%10d ┃ ", proc.dwPID) << flush;
		qout << proc.strProcName << endl;
	}

	qout << qtr("===========================") << endl;
}

//将内存中的数据dump到文件
void CDebuggerMain::dump2file(HANDLE hProc, ULONG64 pStartAddr, ULONG64 pEndAddr, QString strFile)
{
	SIZE_T nBytes = pEndAddr - pStartAddr + 1;
	BYTE *pData = new BYTE[nBytes]{};

	SIZE_T bytesRead = 0;
	if (ReadProcessMemory(hProc, (LPCVOID)pStartAddr, (LPVOID)pData, nBytes, &bytesRead)) {
		QFile tFile(strFile);
		if (tFile.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
			QDataStream outs(&tFile);
			outs.writeRawData((char*)pData, bytesRead);
			tFile.close();
		}
		else {
			qout << qtr("打开文件失败") << endl;
		}
	}
	else {  //读取失败
		DBGPRINT("读取进程内存失败");
	}

	delete[] pData; pData = nullptr;
}

//获取所有的32位进程的信息
void CDebuggerMain::getAll32Process()
{
	HANDLE hProcessSnap = NULL;
	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32 = {};
	UINT uPID;
	BOOL Is32 = FALSE;

	int i = 0;

	if ((hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hProcessSnap, &pe32);

	if (!bRet) {
		//TODO:	
		CloseHandle(hProcessSnap);
		return;
	}
	else {
		do {
			PROCESS tProc = {};
			if (pe32.th32ProcessID == GetCurrentProcessId()) {
				continue;
			}
			else if (pe32.th32ProcessID == 0) {
				continue;
			}
			else {
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
				if (hProcess) {
					IsWow64Process(hProcess, &Is32);
					if (Is32) {
						tProc.dwPID = pe32.th32ProcessID;
						tProc.strProcName = QString::fromWCharArray(pe32.szExeFile);
						m_vtProcess.push_back(tProc);
						CloseHandle(hProcess);
					}
				}
				else {
					continue;
				}
			}
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
	}
}

//寻找并获取指定基址的模块信息
bool CDebuggerMain::findModuleInCurProcess(DWORD dwBaseAddr, EXECMODULE *pModule)
{
	for (auto &mod : m_vtModule) {
		if (dwBaseAddr == mod.dwModBaseAddr) {
			pModule->dwModBaseAddr = mod.dwModBaseAddr;
			pModule->dwModSize = mod.dwModSize;
			pModule->strModPath = mod.strModPath;
			return true;
		}
	}

	return false;
}

//权限提升
bool CDebuggerMain::getSeDebugPrivilge()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	//获取SEDEBUG特权的LUID
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//获取这个进程的关机特权
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return false;

	return true;
}
