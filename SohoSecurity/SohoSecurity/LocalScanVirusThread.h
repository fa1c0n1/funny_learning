#pragma once

#include <QThread>

#ifndef LOCAL_VIRUS_DB
#define LOCAL_VIRUS_DB         TEXT("VirusMD5.txt")
#endif

class LocalScanVirusThread : public QThread
{
	Q_OBJECT

public:
	LocalScanVirusThread(QObject *parent);
	~LocalScanVirusThread();

	void setScanDir(QString strDir);

protected:
	void run();

private:
	void loadLocalVirusDB(QString strFilePath);
	void localScanVirus(QString strDir);

signals:
	void updateLocalScanProgress(QString strFilePath);
	void catchVirus(QString strFilePath);
	

private:
	QString m_strScanDir;
	QVector<QString> m_vtLocalVirusMD5;
};
