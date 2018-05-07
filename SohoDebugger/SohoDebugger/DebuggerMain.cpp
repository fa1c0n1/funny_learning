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

	m_dwDebuggeePID = stcProcInfo.dwProcessId;

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
				qout << qtr("ȱ�ٵ�ַ") << endl;
			}
			else if (cmdList.size() < 3) {
				bool bOK = true;
				LPVOID pAddr = (LPVOID)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					BREAKPOINT bp = {};
					if (!setBreakpointCC(hProcess, pAddr, &bp)) {
						DBGPRINT("���öϵ�ʧ��");
					}
					else {
						m_listBp.push_back(bp);
					}
				}
				else {
					qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bh")) { //bh:����Ӳ���ϵ�
			if (cmdList.size() < 2) {
				qout << qtr("ȱ�ٵ�ַ") << endl;
			}
			else if (cmdList.size() < 3) {
				bool bOK = true;
				ULONG_PTR pAddr = (ULONG_PTR)cmdList[1].toULong(&bOK, 16);
				if (bOK) {
					if (!setBreakpointHardExec(hThread, pAddr))
						DBGPRINT("����Ӳ��ִ�жϵ�ʧ��");
				}
				else {
					qout << qtr("%1����һ���Ϸ���16����ֵ").arg(cmdList[1]) << endl;
				}
			}
		}
		else if (cmdList[0] == qtr("bm")) { //bm:�����ڴ�ϵ�

		}
		else if (cmdList[0] == qtr("lm")) { //lm:�鿴��ִ��ģ����Ϣ
			showExecuteModuleInfo();
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
	ct.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &ct))
		DBGPRINT("��ȡ�߳�������ʧ��");

	qout << qtr("�Ĵ�����Ϣ:") << endl;
	qout << QString::asprintf("EAX:%08X ECX:%08X EDX:%08X EBX:%08X EBP:%08X ESP:%08X",
		ct.Eax, ct.Ecx, ct.Edx, ct.Ebx, ct.Ebp, ct.Esp) << endl;
	qout << QString::asprintf("ESI:%08X EDI:%08X EIP:%08X", ct.Esi, ct.Edi, ct.Eip) << endl;
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
				DBGPRINT("д������ڴ�ʧ��");
				return;
			}
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
bool CDebuggerMain::setBreakpointHardExec(HANDLE hThread, ULONG_PTR pAddr)
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
	}
	else if (pDr7->L1 == 0) { //DR1û�б�ʹ��
		ct.Dr1 = pAddr;
		pDr7->RW1 = 0;
		pDr7->LEN1 = 0;
		pDr7->L1 = 1;
	}
	else if (pDr7->L2 == 0) { //DR2û�б�ʹ��
		ct.Dr2 = pAddr;
		pDr7->RW2 = 0;
		pDr7->LEN2 = 0;
		pDr7->L2 = 1;
	}
	else if (pDr7->L3 == 0) { //DR3û�б�ʹ��
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
	}
	else if (pDr7->L1 == 0) { //DR1 û�б�ʹ��
		ct.Dr1 = pAddr;
		pDr7->RW1 = type;
		pDr7->LEN1 = dwLen;
	}
	else if (pDr7->L2 == 0) { //DR2 û�б�ʹ��
		ct.Dr2 = pAddr;
		pDr7->RW2 = type;
		pDr7->LEN2 = dwLen;
	}
	else if (pDr7->L3 == 0) { //DR3 û�б�ʹ��
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

