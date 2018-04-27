#include "stdafx.h"
#include "LocalScanVirusThread.h"
#include "Md5.h"

LocalScanVirusThread::LocalScanVirusThread(QObject *parent)
	: QThread(parent)
{
	loadLocalVirusDB(QString::fromWCharArray(LOCAL_VIRUS_DB));
}

LocalScanVirusThread::~LocalScanVirusThread()
{
}

void LocalScanVirusThread::setScanDir(QString strDir)
{
	m_strScanDir = strDir;
}

void LocalScanVirusThread::run()
{
	localScanVirus(m_strScanDir);
	emit updateLocalScanProgress(tr("…®√ËÕÍ±œ"));
}

void LocalScanVirusThread::localScanVirus(QString strDir)
{
	QDir scanDir(strDir);

	for (QFileInfo fileInfo : scanDir.entryInfoList()) {
		if (fileInfo.isFile()) {
			QString strFilePath = fileInfo.absoluteFilePath();
			char *pFileMD5 = md5FileValue((char*)strFilePath.toStdString().c_str());
			if (m_vtLocalVirusMD5.contains(tr(pFileMD5))) {
				emit catchVirus(strFilePath);
			}
			else {
				emit updateLocalScanProgress(strFilePath);
			}

			QThread::msleep(30);
		}
		else {
			if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." || fileInfo.isSymLink()) {
				continue;
			}

			localScanVirus(fileInfo.absoluteFilePath());
		}
	}
}

void LocalScanVirusThread::loadLocalVirusDB(QString strFilePath)
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