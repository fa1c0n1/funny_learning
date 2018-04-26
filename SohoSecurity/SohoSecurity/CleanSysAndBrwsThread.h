#pragma once

#include <QThread>

class CleanSysAndBrwsThread : public QThread
{
	Q_OBJECT

public:
	CleanSysAndBrwsThread(QObject *parent);
	~CleanSysAndBrwsThread();

protected:
	void run();

private:
	void cleanTmpFiles();
	void cleanSysFiles();
	void cleanRecycleBin();
	void cleanSysCache();
	void cleanLog();
	void cleanBrowserCache();

signals:
	void updateClnTmpFilesProgress(QString strFilePath);
	void updateClnSysFilesProgress(QString strFilePath);
	void updateClnRecycleBinProgress(QString strFilePath);
	void updateClnSysCacheProgress(QString strFilePath);
	void updateClnLogProgress(QString strFilePath);
	void updateClnBrowserProgress(QString strFilePath);

private:
	QList<QString> m_tmpDirList;
	QList<QString> m_sysDirList;
	QList<QString> m_sysCacheDirList;
	QList<QString> m_sysCacheDirDelAllList;
	QList<QString> m_logDirList;
	QList<QString> m_brwsCacheDirList;

	static TCHAR *m_szTmpFilesSuffix[3];
	static TCHAR *m_szSysCacheSuffix[2];
	static TCHAR *m_szLogSuffix[4];
};
