#include "stdafx.h"
#include "CpuMemInfoThread.h"

CpuMemInfoThread::CpuMemInfoThread(QObject *parent)
	: QThread(parent), m_bQuitThread(false)
{
}

CpuMemInfoThread::~CpuMemInfoThread()
{
}

void CpuMemInfoThread::setQuitThreadFlag(bool bQuitThread)
{
	m_bQuitThread = bQuitThread;
}

void CpuMemInfoThread::run()
{
	int nCpuUsage = 0;
	DWORD dwMemLoad = 0;
	while (!m_bQuitThread) {
		dwMemLoad = getMemoryUsage();
		emit updateMemUsage(dwMemLoad);
		Sleep(100);
		nCpuUsage = getCpuUsage();
		emit updateCpuUsage(nCpuUsage);
	}
}

int CpuMemInfoThread::getCpuUsage()
{
	_FILETIME idleTime, kernelTime, userTime;   //����ʱ�䣬�ں�ʱ�䣬�û�ʱ��

	//��ȡʱ��
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//�ȴ� 1000 ����, ʹ���ں˶���ȴ������ȷ
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);

	//��ȡ��ʱ��
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	//������ʱ��ת��
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);

	//�����ʹ����
	return int(100.0 - (dNewIdleTime - dOldIdleTime) /
		(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}

DWORD CpuMemInfoThread::getMemoryUsage()
{
	MEMORYSTATUS memStatus = {};
	::GlobalMemoryStatus(&memStatus);
	return memStatus.dwMemoryLoad;
}

double CpuMemInfoThread::FILETIME2Double(const _FILETIME &fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}
