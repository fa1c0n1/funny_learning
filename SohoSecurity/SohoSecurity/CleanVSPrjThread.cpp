#include "stdafx.h"
#include "CleanVSPrjThread.h"

CleanVSPrjThread::CleanVSPrjThread(QObject *parent)
	: QThread(parent)
{
}

CleanVSPrjThread::CleanVSPrjThread(QList<QString> prjDirList, QObject *parent)
	: QThread(parent)
{
	m_prjDirList = prjDirList;
}

CleanVSPrjThread::~CleanVSPrjThread()
{
}

void CleanVSPrjThread::run()
{
	for (QString strPrjPath : m_prjDirList) {
		cleanVSPrj(strPrjPath);
	}

	emit updateClnVSPrjProgress(tr("----- 清理完毕 -----"));
}

void CleanVSPrjThread::cleanVSPrj(QString strPrjPath)
{
	QDir prjDir(strPrjPath);

	for (QFileInfo fileInfo : prjDir.entryInfoList()) {
		if (fileInfo.isFile()) {
			for (int i = 0; i < _countof(m_szDelSuffix); i++) {
				if (fileInfo.suffix() == QString::fromWCharArray(m_szDelSuffix[i])) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					if (QFile::remove(strDelFilePath)) {
						emit updateClnVSPrjProgress(tr("删除成功:%1").arg(strDelFilePath));
					}
					else {
						emit updateClnVSPrjProgress(tr("删除失败:%1").arg(strDelFilePath));
					}
					QThread::msleep(30);
					break;
				}
			}
		}
		else {
			if (fileInfo.fileName() == "." || fileInfo.fileName() == ".."
				|| fileInfo.isHidden() || fileInfo.isSymLink()) {
				continue;
			}

			cleanVSPrj(fileInfo.absoluteFilePath());
		}
	}
}

//以下后缀的文件是要获取/删除的文件
//.obj\.tlog\.lastbuildstate\.idb\.pdb\.pch\.res\.ilk\.exe\.sdf\.ipch\.log
TCHAR *CleanVSPrjThread::m_szDelSuffix[] = {
	TEXT("obj"), TEXT("tlog"), TEXT("lastbuildstate"), TEXT("idb"), TEXT("pdb"),
	TEXT("pch"), TEXT("res"), TEXT("ilk"), TEXT("exe"), TEXT("sdf"), TEXT("ipch"), TEXT("log")
};