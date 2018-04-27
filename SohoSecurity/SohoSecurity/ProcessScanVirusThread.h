#pragma once

#include <QThread>

#ifndef LOCAL_VIRUS_DB
#define LOCAL_VIRUS_DB         TEXT("VirusMD5.txt")
#endif

class ProcessScanVirusThread : public QThread
{
	Q_OBJECT

public:
	ProcessScanVirusThread(QObject *parent);
	~ProcessScanVirusThread();

protected:
	void run();

private:
	void scanProcess();
	bool endProcess(DWORD dwPID);
	bool getSeDebugPrivilge();
	void loadLocalVirusDB(QString strFilePath);

signals:
	void updateProcScanProgress(QString strProcName);
	void catchVirusProcess(QString strProcName, QString strPath, QString strState);

private:
	QVector<QString> m_vtLocalVirusMD5;
};
