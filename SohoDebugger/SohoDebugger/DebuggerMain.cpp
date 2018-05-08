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
	m_bGo(false), m_bTmpCC(false), m_bAttach(false)
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
	qout << qtr("��ѡ��(1/2): ") << flush;
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

		}
		else {
			system("cls");
			goto _BEGIN;
		}
	}
	
	startDebug(choiceCmdList[0]);
}

void CDebuggerMain::showMainMenu()
{
	qout << qtr(" ������������ ") << endl;
	qout << qtr("��1.���ļ���") << endl;
	qout << qtr("��2.���ӽ��̩�") << endl;
	qout << qtr(" ������������ ") << endl;
}

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
	return bRet;
}

bool CDebuggerMain::openProc(DWORD dwPID)
{
	getSeDebugPrivilge();
	return DebugActiveProcess(dwPID);
}

void CDebuggerMain::startDebug(QString strChoice)
{
	if (strChoice == qtr("1")) {
		qout << qtr("�������ļ�·��: ") << flush;
		m_strFile = qin.readLine();
		if (!openProc(m_strFile)) {
			qout << qtr("�������Խ���ʧ��") << endl;
			return;
		}
	}
	else if (strChoice == qtr("2")) {
		QString strTmp;
		qout << qtr("���������ID: ") << flush;
		strTmp = qin.readLine();
		bool bOK = true;
		m_dwDebuggeePID = strTmp.toULong(&bOK, 10);
		if (bOK) {
			if (!openProc(m_dwDebuggeePID)) {
				qout << qtr("���ӽ���ʧ��") << endl;
				return;
			}
			else {
				m_bAttach = true;
			}
		}
		else {
			qout << qtr("�������") << endl;
			return;
		}
	}
	else {
		qout << qtr("�����˳�") << endl;
		return;
	}
	
	DEBUG_EVENT debugEvent = { 0 };
	DWORD dwRet = DBG_CONTINUE;

	while (true) {
		//�ȴ������¼�
		//  ��������Խ��̲����˵����¼��������ͻ�
		//  ����Ӧ����Ϣ������ṹ������У����Ӻ����з���,
		//  ��������Խ���û�е����¼��������ᴦ������״̬ 
		WaitForDebugEvent(&debugEvent, INFINITE);

		switch (debugEvent.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
		{
			if (!m_bAttach) {
				BREAKPOINT bp = {};
				setBreakpointCC(debugEvent.u.CreateProcessInfo.hProcess,
					debugEvent.u.CreateProcessInfo.lpStartAddress, &bp);
				m_listBp.push_back(bp);
			}
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

		//�ظ������¼��Ĵ�����
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

	//�����Խ��̲����ĵ�һ���쳣�¼������
	//  �쳣�¼�����ϵͳ�ϵ�
	qout << QString::asprintf("\t�쳣����: %08X", er.ExceptionCode) << endl;
	qout << QString::asprintf("\t�쳣��ַ: %08X", er.ExceptionAddress) << endl;

	if (m_bSystemBreakpoint) {
		qout << qtr("����ϵͳ�ϵ�,����") << endl;
		m_bSystemBreakpoint = false;
		
		if (m_bAttach)
			userInput(hProcess, hThread, er.ExceptionAddress);

		return DBG_CONTINUE;
	}

	switch (er.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT: //����ϵ�
	{
		clearAllBreakpoint(hProcess);
		//���������Ϣ
		showDebugInfo(hProcess, hThread, er.ExceptionAddress);
		if (m_bTmpCC)
			m_listBp.pop_back();
		CONTEXT ct = {};
		getDebuggeeContext(&ct);
		ct.Eip--;
		setDebuggeeContext(&ct);
	}
		break;
	case EXCEPTION_ACCESS_VIOLATION:  //�ڴ�����쳣(�ڴ���ʶϵ�)
		break;

	//TF��Ӳ���ϵ��쳣
	//  ͨ��DR6�Ĵ�����һ���ж�����쳣��
	//  TF�����Ļ��� DR0~DR3 ������
	case EXCEPTION_SINGLE_STEP:
		//���������Ϣ
		showDebugInfo(hProcess, hThread, er.ExceptionAddress);

		if (isHardBreakpoint(hThread)) {
			break;
		}
		
		//�����еĶϵ㶼�ָ�
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
	// �����û�����
	// 1. ��ʾ��Ϣ
	// 1.1 ��ʾ�Ĵ�����Ϣ
	// 1.2 ��ʾջ��Ϣ
	// 1.3 ��ʾָ����ַ�������Ϣ
	// 1.4 ��ʾָ����ַ�ϵ��ڴ�������Ϣ
	// 2. �������
	// 2.1 ��������
	// 2.2 �¶ϵ�
	// 2.2.1 ����ϵ�
	// 2.2.2 Ӳ���ϵ�
	// 2.2.3 �ڴ���ʶϵ�
	// 2.3 ֱ������
	QString strCmdLine;

	m_bGo = false;
	m_bTmpCC = false;
	while (true) {
		qout << qtr(">> ") << flush;
		strCmdLine = qin.readLine();
		if (strCmdLine.isEmpty())
			continue;

		QStringList cmdList = strCmdLine.split(' ', QString::SkipEmptyParts);
		
		if (cmdList[0] == qtr("t")) { //t:��������
			//TF�ϵ�ԭ��:	
			//  ��һ������Ҫ����ָ��ʱ��CPU
			//  ����EFLAGS��TF��־λ�Ƿ�Ϊ1
			//  �����1����CPU����������һ��Ӳ���ϵ��쳣

			setBreakpointTF(hThread);
			//m_bUserTF = true;
			break;
		}
		else if (cmdList[0] == qtr("n")) { //n:��������
			//���жϵ�ǰEIPָ���ָ���Ƿ�ΪCALLָ��
			CONTEXT tContext;
			getDebuggeeContext(&tContext);
			int nCallLen = isCallInstruction(tContext.Eip);

			if (nCallLen) { //��callָ�������һ��ָ���������ϵ�
				BREAKPOINT bp = {};
				if (!setBreakpointCC(hProcess, (LPVOID)(tContext.Eip + nCallLen), &bp)) {
					DBGPRINT("���öϵ�ʧ��");
				}
				else {
					bp.bTmp = true;
					m_bTmpCC = true;
					m_listBp.push_back(bp);
					break;
				}
			}
			else { //����callָ�������TF
				setBreakpointTF(hThread);
				//m_bUserTF = true;
				break;
			}
		}
		else if (cmdList[0] == qtr("b")) {  //b:��������ϵ�
			if (cmdList.size() < 2) {
				showAllBreakpointCCInfo();
			}
			else if (cmdList.size() < 3) { //b <addr> ��������ϵ�
				bool bOK = true;
				LPVOID pAddr = (LPVOID)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					BREAKPOINT bp = {};
					if (!setBreakpointCC(hProcess, pAddr, &bp)) {
						DBGPRINT("��������ϵ�ʧ��");
					}
					else {
						m_listBp.push_back(bp);
					}
				}
				else {
					qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[1]) << endl;
				}
			}
			else if (cmdList.size() < 4) { //b -d <addr> ɾ������ϵ�
				if (cmdList[1] == qtr("-d")) {
					bool bOK = true;
					LPVOID pAddr = (LPVOID)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!rmBreakpointCC(hProcess, pAddr))
							DBGPRINT("ɾ������ϵ�ʧ��");
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("��������") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bh")) { //bh:����Ӳ���ϵ�
			BREAKPOINTHARD bph = {};
			if (cmdList.size() < 2) {
				showAllBreakpointHardInfo();
			}
			else if (cmdList.size() < 3) {
				qout << qtr("ȱ�ٲ���") << endl;
			}
			else if (cmdList.size() < 4) {  
				if (cmdList[1] == qtr("-e")) { //bh [-e] <addr> ����Ӳ��ִ�жϵ�
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointHardExec(hThread, pAddr, &bph))
							DBGPRINT("����Ӳ��ִ�жϵ�ʧ��");
						else
							m_listBpHard.push_back(bph);
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[2]) << endl;
					}
				}
				else if (cmdList[1] == qtr("-d")) { //bh [-d] <addr> ɾ��Ӳ���ϵ�
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (m_listBpHard.isEmpty()) {
							qout << qtr("δ����Ӳ���ϵ�") << endl;
						}
						else {
							if (!rmBreakpointHard(hThread, pAddr))
								DBGPRINT("ɾ��Ӳ���ϵ�ʧ��");
						}
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("��������") << endl;
				}
				
			}
			else if (cmdList.size() < 5) { //bh [-rw] <addr> <len> ����Ӳ����д�ϵ�
				if (cmdList[1] == qtr("-rw")) {
					bool bOK = true;
					ULONG_PTR pAddr = (ULONG_PTR)cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						DWORD dwLen = (DWORD)cmdList[3].toULong(&bOK, 10);
						if (bOK) {
							if (dwLen != 1 && dwLen != 2 && dwLen != 4) {
								qout << qtr("����ֻ����1��2��4") << endl;
							}
							else {
								if (!setBreakpointHardRW(hThread, pAddr, HARDBP_READWRITE, dwLen - 1, &bph))
									DBGPRINT("����Ӳ����д�ϵ�ʧ��");
								else
									m_listBpHard.push_back(bph);
							}
						}
						else {
							qout << qtr("%1����һ���Ϸ���10����ֵ").arg(cmdList[3]) << endl;
						}
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[2]) << endl;
					}
				}
				else {
					qout << qtr("��������") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bm")) { //bm:�����ڴ�ϵ�
			if (cmdList.size() < 2) {
				showAllBreakpointMemInfo();
			}
			else if (cmdList.size() < 3) {
				qout << qtr("ȱ�ٲ���") << endl;
			}
			else if (cmdList.size() < 4) { 
				BREAKPOINTMEM bpm = {};
				if (cmdList[1] == qtr("-a")) { //bm [-a] <addr> �����ڴ���ʶϵ�
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointMem(hProcess, pAddr, PAGE_NOACCESS, &bpm))
							DBGPRINT("�����ڴ���ʶϵ�ʧ��");
						else
							m_listBpMem.push_back(bpm);
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ") << endl;
					}
				}
				else if (cmdList[1] == qtr("-w")) { //bm [-w] <addr> �����ڴ�д��ϵ�
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (!setBreakpointMem(hProcess, pAddr, PAGE_READONLY, &bpm))
							DBGPRINT("�����ڴ�д��ϵ�ʧ��");
						else
							m_listBpMem.push_back(bpm);
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ") << endl;
					}
				}
				else if (cmdList[1] == qtr("-d")) { //bm [-d] <addr> ɾ���ڴ�ϵ�
					bool bOK = true;
					ULONG_PTR pAddr = cmdList[2].toULong(&bOK, 16);
					if (bOK) {
						if (m_listBpMem.isEmpty()) {
							qout << qtr("δ�����ڴ�ϵ�") << endl;
						}
						else {
							if (!rmBreakpointMem(hProcess))
								DBGPRINT("ɾ���ڴ�ϵ�ʧ��");
							else
								m_listBpMem.pop_back();
						}
					}
					else {
						qout << qtr("%1����һ���Ϸ���16����ֵ") << endl;
					}
				}
				else {
					qout << qtr("��������") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("lm")) { //lm:�鿴��ִ��ģ����Ϣ
			showExecuteModuleInfo();
		}
		else if (cmdList[0] == qtr("stack")) { //stack:�鿴ջ��Ϣ
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
					qout << qtr("%1����һ���Ϸ���10����ֵ").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("r")) { //r: �鿴/�޸ļĴ���
			if (cmdList.size() < 2) { //r �鿴���мĴ�����ֵ
				showRegisterInfo(hThread);
			}
			else if (cmdList.size() < 3) { //r [reg] �޸ļĴ�����ֵ
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
						qout << qtr("%1 ���ǺϷ���16����ֵ").arg(strRegVal) << endl;
					}
				}
				else {
					qout << qtr("%1:�����ڸüĴ���").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("u")) { //u:�鿴��������
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
		else if (cmdList[0] == qtr("d")) { //d:�鿴�ڴ�����
			if (cmdList.size() < 2) {
				showMemoryInfo(hProcess, pExceptionAddr, 128);
			}
			else if (cmdList.size() < 3) { //d [addr]
				bool bOK = true;
				DWORD dwAddr = cmdList[1].toULong(&bOK, 16);
				if (bOK)
					showMemoryInfo(hProcess, (LPVOID)dwAddr, 128);
				else
					qout << qtr("����ĵ�ַ���Ϸ�") << endl;
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
					qout << qtr("����ĵ�ַ���Ϸ�") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("e")) { //e:�޸��ڴ��е�����
			if (cmdList.size() < 3) {
				qout << qtr("ȱ�ٲ���") << endl;
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
							qout << qtr("%1 ���ǺϷ���16������ֵ").arg(cmdList[nIdx]) << endl;
							vtData.clear();
							break;
						}
					}

					if (vtData.size() > 0)
						editMemoryInfo(hProcess, (LPVOID)dwAddr, vtData);

				}
				else {
					qout << qtr("����ĵ�ַ���Ϸ�") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("a")) { //a:д����ָ��
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
					qout << qtr("����ĵ�ַ���Ϸ�") << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("g")) { //g:��������
			m_bGo = true;
			setBreakpointTF(hThread);
			m_bUserTF = true;
			break;
		}
	}
}

//���ָ����ַ����ָ����CALL������CALLָ��ĳ���
//���򷵻�0
//�жϵķ����ο��ˡ�CALLָ���ж�����д����һ��
//http://blog.ftofficer.com/2010/04/n-forms-of-call-instructions
int CDebuggerMain::isCallInstruction(DWORD dwAddr)
{
	BYTE instruct[20] = {};
	DWORD dwNumOfByteRead = 0;
	if (!ReadProcessMemory(m_hProcess, (LPVOID)dwAddr, instruct, 20, &dwNumOfByteRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
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
		DBGPRINT("�����߳�������ʧ��");
}

void CDebuggerMain::getDebuggeeContext(PCONTEXT pContext)
{
	pContext->ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(m_hThread, pContext))
		DBGPRINT("��ȡ�߳�������ʧ��");
}

void CDebuggerMain::getNextInstructAddr(HANDLE hProcess, LPVOID pAddr, LPVOID *pNextAddr)
{
	LPBYTE opcode[200] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProcess, pAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("��ȡ�ڴ�ʧ��");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pAddr;
		disAsm.Archi = 0;  //x86���
		int nLen = 0;

		//nLen���ص��Ƿ���������ָ��Ļ������ֽ���
		// ��������ʧ�ܣ��򷵻�-1
		nLen = Disasm(&disAsm);
		if (nLen == -1) {
			pNextAddr = nullptr;
			DBGPRINT("�����ʧ��");
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
		DBGPRINT("��ȡ�߳�������ʧ��");
	
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
			DBGPRINT("�����߳�������ʧ��");
	}
	else {
		qout << qtr("%1 �����ڸüĴ���").arg(strRegName) << endl;
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
		if (nRet == -1) { //���ʧ��
		}
		else {
			//��ָ��д���ڴ�
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
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("��ȡ�߳�������ʧ��");

	qout << qtr("�Ĵ�����Ϣ:") << endl;
	qout << QString::asprintf("EAX:%08X ECX:%08X EDX:%08X EBX:%08X EBP:%08X ESP:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx, ct.Ebp, ct.Esp) << endl;
	qout << QString::asprintf("ESI:%08X EDI:%08X EIP:%08X", ct.Esi, ct.Edi, ct.Eip) << endl;

	
	qout << qtr("���ԼĴ�����Ϣ:") << endl;
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

void CDebuggerMain::showExecuteModuleInfo()
{
	if (m_vtModule.isEmpty())
		traverseExecModule(m_dwDebuggeePID);

	qout << qtr("��ִ��ģ��: ") << endl;
	qout << qtr("��ַ����С��·��") << endl;
	for (auto &execMod : m_vtModule) {
		qout << QString::asprintf("%08X��%08X��", 
			execMod.dwModBaseAddr, execMod.dwModSize) << execMod.strModPath << endl;
	}
}

void CDebuggerMain::showStackInfo(HANDLE hProc, HANDLE hThread, int nCnt)
{
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("��ȡ�߳�������ʧ��");

	DWORD *pBuf = new DWORD[nCnt]{};
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, (LPCVOID)ct.Esp, pBuf, nCnt*sizeof(DWORD), &bytesRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
	}
	else {
		qout << qtr("ջ����: \n");
		for (int i = 0; i < nCnt; i++) {
			qout << QString::asprintf("%08X��%08X",
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
			qout << QString::asprintf("%08X��", pCurAddr) << flush;
		}

		if (ReadProcessMemory(hProc, (LPVOID)pCurAddr, &data, 1, &byteRead)) {
			if ((strAsciiLine.size() + 1) % 4 == 0)
				qout << QString::asprintf("%02X��", data) << flush;
			else
				qout << QString::asprintf("%02X ", data) << flush;
			if (data >= 0x00 && data <= 0x1F)
				strAsciiLine.append(".");
			else if (data >= 0x81 && data <= 0xFF)
				strAsciiLine.append("?");
			else
				strAsciiLine.append((char)data);
		}
		else {  //��ȡʧ��
			qout << qtr("?? ");
			strAsciiLine.append(".");
		}

		//���һ���� 16 ���ֽ�,�����ASCII�ַ�,���������
		if (strAsciiLine.size() == 16) {
			qout << strAsciiLine << endl;
			strAsciiLine.clear();
		}

		pCurAddr++;
	}

	if (nExtraBytes > 0) {
		for (int i = 0; i < 16 - nExtraBytes; i++) {
			if ((i+1+nExtraBytes) % 4 == 0)
				qout << qtr("**��") << flush;
			else
				qout << qtr("** ") << flush;
		}
		qout << strAsciiLine << endl;
	}
}

void CDebuggerMain::showDebugInfo(HANDLE hProc, HANDLE hThread, LPVOID pExceptionAddr)
{
	//����Ĵ�����Ϣ
	showRegisterInfo(hThread);

	//���ջ��Ϣ
	showStackInfo(hProc, hThread, 10);

	//����������Ϣ
	showDisambleInfo(hProc, pExceptionAddr, 10);
}

void CDebuggerMain::showDisambleInfo(HANDLE hProc, LPVOID pAddr, int nCnt)
{
	BYTE opcode[200] = { 0 };
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, pAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("��ȡ�ڴ�ʧ��");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pAddr;
		disAsm.Archi = 0;  //x86���
		int nLen = 0;

		//nLen���ص��Ƿ���������ָ��Ļ������ֽ���
		// ��������ʧ�ܣ��򷵻�-1
		qout << qtr("�������Ϣ: \n");
		int nSize = 0;
		while (nSize < nCnt) {
			nLen = Disasm(&disAsm);
			if (nLen == -1)
				break;

			//��������
			qout << QString::asprintf("0x%08X��%s",
				(DWORD)disAsm.VirtualAddr, disAsm.CompleteInstr) << endl;

			disAsm.EIP += nLen;
			disAsm.VirtualAddr += nLen;
			++nSize;
		}
	}
}

void CDebuggerMain::resetAllBreakpoint(HANDLE hProcess)
{
	//�ָ���������ϵ�
	BREAKPOINT tBp = {};
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			setBreakpointCC(hProcess, bp.pAddr, &tBp);
		}
	}

	//�ָ�����Ӳ���ϵ�
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_hThread, &ct);
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
	SetThreadContext(m_hThread, &ct);
}

void CDebuggerMain::clearAllBreakpoint(HANDLE hProcess)
{
	//ɾ����������ϵ�
	for (auto &bp : m_listBp) {
		if (bp.dwType == EXCEPTION_BREAKPOINT) {
			DWORD dwRead = 0;
			if (!WriteProcessMemory(hProcess, bp.pAddr, &bp.oldData, 1, &dwRead)) {
				DBGPRINT("д������ڴ�ʧ��");
				return;
			}
		}
	}

	//ɾ������Ӳ���ϵ�
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_hThread, &ct);
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
	SetThreadContext(m_hThread, &ct);
}

bool CDebuggerMain::setBreakpointCC(HANDLE hProc, LPVOID pAddr, BREAKPOINT *bp)
{
	/*
	  ԭ��:
	     ��ָ����ַ��д�� 0xCC(int 3 ָ��Ļ�����),��
		 ����ִ�� int 3 ָ���ʱ�򣬾ͻ�����쳣����������
		 �ܽ��յ����쳣(Ҳ�����׳ƵĶ�����)
	*/
		
	bp->pAddr = pAddr;
	bp->dwType = EXCEPTION_BREAKPOINT;

	//1.��������
	SIZE_T dwRead = 0;
	if (!ReadProcessMemory(hProc, pAddr, &bp->oldData, 1, &dwRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
		return false;
	}

	//2.д��CC
	if (!WriteProcessMemory(hProc, pAddr, "\xCC", 1, &dwRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
		return false;
	}

	return true;
}

//��һ����������Ķϵ�
void CDebuggerMain::setBreakpointTF(HANDLE hThread)
{
	//1.��ȡ�߳�������
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct)) {
		DBGPRINT("��ȡ�̻߳���ʧ��");
	}

	//2.����EFLAGS��TF��־λ
	PEFLAGS pEflags = (PEFLAGS)&ct.EFlags;
	pEflags->TF = 1;

	//3.�����߳�������
	if (!SetThreadContext(hThread, &ct)) {
		DBGPRINT("�����̻߳���ʧ��");
	}

	//ɾ�����жϵ�
	clearAllBreakpoint(m_hProcess);
}

//����Ӳ��ִ�жϵ�
bool CDebuggerMain::setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr, BREAKPOINTHARD *bph)
{
	/*
	  ����Ӳ��ִ�жϵ�ʱ������ֻ��Ϊ1(LEN0~LEN3����Ϊ0ʱ��ʾ����Ϊ1)	
	*/
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(hThread, &ct);
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;

	if (pDr7->L0 == 0) { //DR0û�б�ʹ��
		ct.Dr0 = pAddr;
		pDr7->RW0 = 0;
		pDr7->LEN0 = 0;
		pDr7->L0 = 1;
		bph->dwDrID = 0;
	}
	else if (pDr7->L1 == 0) { //DR1û�б�ʹ��
		ct.Dr1 = pAddr;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
		pDr7->L1 = 1;
		bph->dwDrID = 1;
	}
	else if (pDr7->L2 == 0) { //DR2û�б�ʹ��
		ct.Dr2 = pAddr;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
		pDr7->L2 = 1;
		bph->dwDrID = 2;
	}
	else if (pDr7->L3 == 0) { //DR3û�б�ʹ��
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

	//�Ե�ַ�ͳ��Ƚ��ж��봦��(����ȡ��)
	if (dwLen == 1) { //2�ֽڵĶ�������
		pAddr = pAddr - pAddr % 2;
	}
	else if (dwLen == 3) { //4�ֽڵĶ�������
		pAddr = pAddr - pAddr % 4;
	}
	else if (dwLen > 3) {
		return false;
	}

	//�ж���Щ�Ĵ���û�б�ʹ��
	DBG_REG7 *pDr7 = (DBG_REG7*)&ct.Dr7;
	if (pDr7->L0 == 0) { //DR0 û�б�ʹ��
		ct.Dr0 = pAddr;
		pDr7->RW0 = type;
		pDr7->LEN0 = dwLen;
		pDr7->L0 = 1;
		bph->dwDrID = 0;
	}
	else if (pDr7->L1 == 0) { //DR1 û�б�ʹ��
		ct.Dr1 = pAddr;
		pDr7->RW1 = type;
		pDr7->LEN1 = dwLen;
		pDr7->L1 = 1;
		bph->dwDrID = 1;
	}
	else if (pDr7->L2 == 0) { //DR2 û�б�ʹ��
		ct.Dr2 = pAddr;
		pDr7->RW2 = type;
		pDr7->LEN2 = dwLen;
		pDr7->L2 = 1;
		bph->dwDrID = 2;
	}
	else if (pDr7->L3 == 0) { //DR3 û�б�ʹ��
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

//ɾ��Ӳ���ϵ�
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
		qout << qtr("δ�ڸõ�ַ������Ӳ���ϵ�") << endl;
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

//�����ڴ�ϵ�
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
	BREAKPOINTMEM bpm = m_listBpMem.at(0);
	DWORD dwTmp = 0;
	if (!VirtualProtectEx(hProcess, (LPVOID)bpm.pAddr, 1, bpm.dwOldType, &dwTmp))
		return false;
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

//ɾ������ϵ�
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
				DBGPRINT("д���ڴ�ʧ��");
				bRet = false;
				break;
			}
		}
	}

	if (!bBpExist) {
		qout << qtr("�õ�ַ������������ϵ�") << endl;
		return false;
	}

	if (bBpExist && bRet)
		m_listBp.removeAt(i);
	
	return bRet;
}

void CDebuggerMain::showAllBreakpointCCInfo()
{
	if (m_listBp.isEmpty()) {
		qout << qtr("δ��������ϵ�") << endl;
		return;
	}

	qout << qtr("����ϵ���Ϣ:") << endl;
	int i = 0;
	for (auto &bp : m_listBp) {
		qout << QString::asprintf("[%d]: ��ַ=%08X", i, bp.pAddr) << endl;
		i++;
	}
}

void CDebuggerMain::showAllBreakpointHardInfo()
{
	if (m_listBpHard.isEmpty()) {
		qout << qtr("δ����Ӳ���ϵ�") << endl;
		return;
	}

	qout << qtr("Ӳ���ϵ���Ϣ:") << endl;
	int i = 0;
	for (auto &bph : m_listBpHard) {
		qout << QString::asprintf("[%d]: ��ַ=%08X ", i, bph.pAddr) << flush;
		QString strType;
		if (bph.dwType == HARDBP_EXEC)
			strType = qtr("Ӳ��ִ��");
		else if (bph.dwType == HARDBP_WRITE)
			strType = qtr("Ӳ��д��");
		else if (bph.dwType == HARDBP_READWRITE)
			strType = qtr("Ӳ����д");
		else
			strType = qtr("δ֪");

		qout << qtr("����=%1 ").arg(strType) << flush;

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
			strDrID = qtr("δ֪");

		qout << qtr("Dr�Ĵ���=%1").arg(strDrID) << endl;
		i++;
	}
}

void CDebuggerMain::showAllBreakpointMemInfo()
{
	if (m_listBpMem.isEmpty()) {
		qout << qtr("δ�����ڴ�ϵ�") << endl;
		return;
	}

	qout << qtr("�ڴ�ϵ���Ϣ:") << endl;

	int i = 0;
	for (auto &bpm : m_listBpMem) {
		qout << QString::asprintf("[%d]: ��ַ=%08X �����ڴ�ҳ:[%08X~%08X] ", 
			i, bpm.pAddr, bpm.pPageStartAddr, bpm.pPageEndAddr) << flush;

		QString strType;
		if (bpm.dwNewType == PAGE_NOACCESS)
			strType = qtr("�ڴ����");
		else if (bpm.dwNewType == PAGE_READONLY)
			strType = qtr("�ڴ�д��");
		else
			strType = qtr("δ֪");

		qout << qtr("����=%1").arg(strType) << endl;

		i++;
	}
}

void CDebuggerMain::showAll32Process()
{
	qout << qtr("������Ϣ: ����ID �� ������") << endl;

	for (auto &proc : m_vtProcess) {
		//qout << QString::asprintf("%d")
	}

	qout << qtr("===========================") << endl;
}

void CDebuggerMain::getAll32Process()
{

}

bool CDebuggerMain::getSeDebugPrivilge()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	//��ȡSEDEBUG��Ȩ��LUID
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//��ȡ������̵Ĺػ���Ȩ
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return false;

	return true;
}
