#pragma once

#include "stdafx.h"
#include <QThread>

class CpuMemInfoThread : public QThread 
{
	Q_OBJECT

public:
	CpuMemInfoThread(QObject *parent);
	~CpuMemInfoThread();

signals:
void updateCpuUsage(int nCpuUsage);
void updateMemUsage(int dwMemUsage);

protected:
	void run();

private:
	int getCpuUsage();
	DWORD getMemoryUsage();
	double FILETIME2Double(const _FILETIME &fileTime);
};
