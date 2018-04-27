#include "stdafx.h"
#include "ProcessScanVirusThread.h"
#include "Md5.h"
#include <TlHelp32.h>

ProcessScanVirusThread::ProcessScanVirusThread(QObject *parent)
	: QThread(parent)
{
	loadLocalVirusDB(QString::fromWCharArray(LOCAL_VIRUS_DB));
}

ProcessScanVirusThread::~ProcessScanVirusThread()
{
}

void ProcessScanVirusThread::scanProcess()
{
	HANDLE hProcessSnap = NULL;
	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32 = {};
	UINT uPID;
	UINT uPPID;
	BOOL Is32 = FALSE;
	QString strProcessFullPath;

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
			if (pe32.th32ProcessID == GetCurrentProcessId()) {
				continue;
			}
			else if (pe32.th32ProcessID == 0) {
				continue;
			}
			else {
				//获取进程对应程序的路径
				getSeDebugPrivilge();
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
				if (hProcess) {
					IsWow64Process(hProcess, &Is32);
					TCHAR tFullPathBuf[MAX_PATH] = {};
					DWORD dwSize = MAX_PATH;
					if (QueryFullProcessImageName(hProcess, 0, tFullPathBuf, &dwSize)) {
						strProcessFullPath = QString::fromWCharArray(tFullPathBuf);
					}
					else {
						strProcessFullPath = "无法获取";
					}
				}
				else {
					strProcessFullPath = "无法获取";
				}
			}

			QString strProcName = QString::fromWCharArray(pe32.szExeFile) + (Is32 ? " *32" : "");
			emit updateProcScanProgress(strProcName);
			
			if (strProcessFullPath != tr("无法获取")) {
				char *pFileMD5 = md5FileValue((char*)strProcessFullPath.toStdString().c_str());
				if (m_vtLocalVirusMD5.contains(tr(pFileMD5))) {
					if (TerminateProcess(hProcess, 0) != 0) {
						emit catchVirusProcess(strProcName, strProcessFullPath, tr("已结束该进程"));
					}
					else {
						emit catchVirusProcess(strProcName, strProcessFullPath, tr("无法结束该进程"));
					}
				}
			}

			QThread::msleep(30);

			i++;
			CloseHandle(hProcess);
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
	}

	emit updateProcScanProgress(tr("扫描完毕"));
}

bool ProcessScanVirusThread::endProcess(DWORD dwPID)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, NULL, dwPID);
	if (hProcess)
		return TerminateProcess(hProcess, 0) != 0;
	else
		return false;
}

bool ProcessScanVirusThread::getSeDebugPrivilge()
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

void ProcessScanVirusThread::run()
{
	scanProcess();
}

void ProcessScanVirusThread::loadLocalVirusDB(QString strFilePath)
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream fileTxtStream(&file);

	QString strLine;
	while (!(strLine = fileTxtStream.readLine()).isNull()) {
		m_vtLocalVirusMD5.push_back(strLine);
	}
}