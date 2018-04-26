#pragma once

#include <QThread>
#include <QList>

class CleanVSPrjThread : public QThread
{
	Q_OBJECT

protected:
	void run();

private:
	void cleanVSPrj(QString strPrjPath);

signals:
	void updateClnVSPrjProgress(QString strFilePath);

public:
	CleanVSPrjThread(QObject *parent);
	CleanVSPrjThread(QList<QString> prjDirList, QObject *parent);
	~CleanVSPrjThread();

private:
	QList<QString> m_prjDirList;
	static TCHAR *m_szDelSuffix[12];
};
