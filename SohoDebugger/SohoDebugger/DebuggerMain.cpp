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
	qout << qtr("�������ļ�·��: ") << flush;
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
		qout << qtr("�������Խ���ʧ��") << endl;
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

		//�ظ������¼��Ĵ�����
		ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, dwRet);
	}
}

DWORD CDebuggerMain::onException(DEBUG_EVENT *pEvent)
{
	EXCEPTION_RECORD &er = pEvent->u.Exception.ExceptionRecord;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEvent->dwProcessId);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pEvent->dwThreadId);

	//�����еĶϵ㶼�ָ�
	setAllBreakpoint(hProcess);

	//�����Խ��̲����ĵ�һ���쳣�¼������
	//  �쳣�¼�����ϵͳ�ϵ�
	qout << QString::asprintf("\t�쳣����: %08X", er.ExceptionCode) << endl;
	qout << QString::asprintf("\t�쳣��ַ: %08X", er.ExceptionAddress) << endl;


	if (m_bSystemBreakpoint) {
		qout << qtr("����ϵͳ�ϵ�") << endl;
		m_bSystemBreakpoint = false;
		userInput(hProcess, hThread);
		return DBG_CONTINUE;
	}

	//����쳣�Ƿ��ǵ�������װ�Ķϵ�������
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

	//���������Ϣ
	showDebugInfo(hProcess, hThread, er.ExceptionAddress);

	switch (er.ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT: //����ϵ�

		break;
	case EXCEPTION_ACCESS_VIOLATION:  //�ڴ�����쳣(�ڴ���ʶϵ�)
		break;

	//TF��Ӳ���ϵ��쳣
	//  ͨ��DR6�Ĵ�����һ���ж�����쳣��
	//  TF�����Ļ��� DR0~DR3 ������
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

	while (true) {
		qout << qtr(">> ") << flush;
		strCmdLine = qin.readLine();
		if (strCmdLine.isEmpty())
			continue;

		//������������
		QStringList cmdList = strCmdLine.split(' ', QString::SkipEmptyParts);
		
		if (cmdList[0] == qtr("t")) { //��������
			//TF�ϵ�ԭ��:	
			//  ��һ������Ҫ����ָ��ʱ��CPU
			//  ����EFLAGS��TF��־λ�Ƿ�Ϊ1
			//  �����1����CPU����������һ��Ӳ���ϵ��쳣

			setBreakpointTF(hThread);

			m_bUserTF = true;
			break;
		}
		else if (cmdList[0] == qtr("bp")) {
			LPVOID pAddr = (LPVOID)cmdList[1].toULong(NULL, 16);
			BREAKPOINT bp = {};
			if (!setBreakpointCC(hProcess, pAddr, &bp)) {
				qout << qtr("���öϵ�ʧ��") << endl;
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
	//����Ĵ�����Ϣ
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("��ȡ�߳�������ʧ��");

	qout << QString::asprintf("\tEAX:%08X ECX:%08X EDX:%08X EBX:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx) << endl;
	qout << QString::asprintf("\tEIP:%08X", ct.Eip) << endl;

	//���ջ��Ϣ
	DWORD dwBuf[5];
	SIZE_T bytesRead = 0;
	if (!ReadProcessMemory(hProc, (LPCVOID)ct.Esp, dwBuf, sizeof(dwBuf), &bytesRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
	}
	else {
		qout << qtr("\tջ����: \n");
		for (int i = 0; i < 5; i++) {
			qout << QString::asprintf("\t%08X|%08X", 
				ct.Esp + i * 4, dwBuf[i]) << endl;
		}
	}

	//����������Ϣ
	LPBYTE opcode[200];
	if (!ReadProcessMemory(hProc, pExceptionAddr, opcode, 200, &bytesRead)) {
		DBGPRINT("��ȡ�ڴ�ʧ��");
	}
	else {
		DISASM disAsm = { 0 };
		disAsm.EIP = (UIntPtr)opcode;
		disAsm.VirtualAddr = (UInt64)pExceptionAddr;
		disAsm.Archi = 0;  //x86���
		int nLen = 0;

		//nLen���ص��Ƿ���������ָ��Ļ������ֽ���
		// ��������ʧ�ܣ��򷵻�-1
		int nSize = 0;
		while (nSize < 5) {
			nLen = Disasm(&disAsm);
			if (nLen == -1)
				break;

			//��������
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
	ct.ContextFlags = CONTEXT_CONTROL;
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
}

bool CDebuggerMain::rmBreakpointCC(HANDLE hProc, HANDLE hThread, LPVOID pAddr, BYTE oldData)
{
	//1.ֱ�ӽ�ԭʼ����д���ȥ
	SIZE_T bytesWrite = 0;
	if (!WriteProcessMemory(hProc, pAddr, &oldData, 1, &bytesWrite)) {
		DBGPRINT("д���ڴ�ʧ��");
		return false;
	}

	//2.��Ϊ int 3 �������쳣������֮��EIP����һ��ָ��ĵ�ַ
	// �����Ҫ�� EIP-1
	CONTEXT ct = {};
	ct.ContextFlags = CONTEXT_CONTROL;
	if (!GetThreadContext(hThread, &ct)) {
		DBGPRINT("��ȡ�̻߳���ʧ��");
		return false;
	}

	ct.Eip--;
	if (!SetThreadContext(hThread, &ct)) {
		DBGPRINT("��ȡ�̻߳���ʧ��");
		return false;
	}

	return true;
}
