#include "stdafx.h"
#include "CleanSysAndBrwsThread.h"

/************************************************************************
临时文件：.dmp, .tmp, .log
C:/Windows
C:/Users/<用户名> [QDir::homePath()] + /AppData/Local

系统文件：
C:/ProgramData/Microsoft/Windows Defender/Scans
C:/ProgramData/Microsoft/Windows Defender/Definition Updates/Backup

回收站

系统缓存：.dat, .db
C:/Windows/ServiceProfiles/LocalService/AppData/Local
C:/Users/<用户名> [QDir::homePath()] + /AppData/Local/Microsoft/Windows/Explorer
C:/Users/<用户名> [QDir::homePath()] + /AppData/Local

[下面目录清理所有文件]
C:/Windows/System32/config/systemprofile/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content
C:/Users/<用户名> [QDir::homePath] + /AppData/LocalLow/Microsoft/CryptnetUrlCache/Content

日志文件：.log, .bak, .txt, .wer
C:/Windows
C:/Windows/Logs
C:/Windows/inf
C:/Windows/System32/catroot2
C:/Windows/debug
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/Microsoft/Windows/WER

浏览器垃圾:
//360Chrome
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/360Chrome/Chrome/User Data/Default/Cache
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/360Chrome/Chrome/User Data/Default/Session Storage
//Google Chrome
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/Google/Chrome/User Data/ShaderCache/GPUCache
C:/Users/fa1c0n [QDir::homePath()] + /Local Settings/application data/google/Chrome/User Data/ShaderCache/GPUCache
//IE
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/Microsoft/Windows/WebCache
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/Microsoft/Windows/Temporary Internet Files
************************************************************************/

CleanSysAndBrwsThread::CleanSysAndBrwsThread(QObject *parent)
	: QThread(parent)
{
	//临时文件
	m_tmpDirList.push_back(tr("C:/Windows"));
	m_tmpDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/Explorer"));

	//系统文件
	m_sysDirList.push_back(tr("C:/ProgramData/Microsoft/Windows Defender/Scans"));
	m_sysDirList.push_back(tr("C:/ProgramData/Microsoft/Windows Defender/Definition Updates/Backup"));

	//系统缓存
	m_sysCacheDirList.push_back(tr("C:/Windows/ServiceProfiles/LocalService/AppData/Local"));
	m_sysCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/Explorer"));
	m_sysCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local"));
	m_sysCacheDirDelAllList.push_back(tr("C:/Windows/System32/config/systemprofile/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content"));
	m_sysCacheDirDelAllList.push_back(QDir::homePath() + tr("/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content"));

	//日志文件
	m_logDirList.push_back(tr("C:/Windows"));
	m_logDirList.push_back(tr("C:/Windows/Logs"));
	m_logDirList.push_back(tr("C:/Windows/inf"));
	m_logDirList.push_back(tr("C:/Windows/System32/catroot2"));
	m_logDirList.push_back(tr("C:/Windows/debug"));
	m_logDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/WER"));

	//浏览器垃圾
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/360Chrome/Chrome/User Data/Default/Cache"));
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/360Chrome/Chrome/User Data/Default/Session Storage"));
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Google/Chrome/User Data/ShaderCache/GPUCache"));
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Google/Chrome/User Data/ShaderCache/GPUCache"));
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/WebCache"));
	m_brwsCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/Temporary Internet Files"));
}

CleanSysAndBrwsThread::~CleanSysAndBrwsThread()
{
}

void CleanSysAndBrwsThread::run()
{
	//清理临时文件
	cleanTmpFiles();
	emit updateClnTmpFilesProgress(tr("清理完毕"));

	//清理系统文件
	cleanSysFiles();
	emit updateClnSysFilesProgress(tr("清理完毕"));

	//清理回收站
	cleanRecycleBin();
	emit updateClnRecycleBinProgress(tr("清理完毕"));

	//清理系统缓存
	cleanSysCache();
	emit updateClnSysCacheProgress(tr("清理完毕"));

	//清理日志
	cleanLog();
	emit updateClnLogProgress(tr("清理完毕"));

	//清理浏览器垃圾
	cleanBrowserCache();
	emit updateClnBrowserProgress(tr("清理完毕"));
}

//清理临时文件
void CleanSysAndBrwsThread::cleanTmpFiles()
{
	for (QString strDirPath : m_tmpDirList) {
		QDir tmpDir(strDirPath);
		if (tmpDir.exists()) {
			for (QFileInfo fileInfo : tmpDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					for (int i = 0; i < _countof(m_szTmpFilesSuffix); i++) {
						if (fileInfo.suffix() == QString::fromWCharArray(m_szTmpFilesSuffix[i])) {
							QString strDelFilePath = fileInfo.absoluteFilePath();
							//QFile::remove(strDelFilePath);
							emit updateClnTmpFilesProgress(tr("删除成功:%1").arg(strDelFilePath));
							QThread::msleep(30);
							break;
						}
					}
				}
			}
		}
	}
}

//清理系统文件
void CleanSysAndBrwsThread::cleanSysFiles()
{
	for (QString strDirPath : m_sysDirList) {
		QDir sysDir(strDirPath);
		if (sysDir.exists()) {
			for (QFileInfo fileInfo : sysDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					//QFile::remove(strDelFilePath);
					emit updateClnSysFilesProgress(tr("删除成功:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//清空回收站
void CleanSysAndBrwsThread::cleanRecycleBin()
{
	//初始化 SHQUERYRBINFO 结构体
	SHQUERYRBINFO recycleBinInfo = {};
	recycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

	//查询回收站信息
	SHQueryRecycleBin(NULL, &recycleBinInfo);

	//清空回收站
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//清理系统缓存
void CleanSysAndBrwsThread::cleanSysCache()
{
	for (QString strDirPath : m_sysCacheDirList) {
		QDir sysCacheDir(strDirPath);
		if (sysCacheDir.exists()) {
			for (QFileInfo fileInfo : sysCacheDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					for (int i = 0; i < _countof(m_szSysCacheSuffix); i++) {
						if (fileInfo.suffix() == QString::fromWCharArray(m_szSysCacheSuffix[i])) {
							QString strDelFilePath = fileInfo.absoluteFilePath();
							//QFile::remove(strDelFilePath);
							emit updateClnSysCacheProgress(tr("删除成功:%1").arg(strDelFilePath));
							QThread::msleep(30);
							break;
						}
					}
				}
			}
		}
	}

	for (QString strDirPath : m_sysCacheDirDelAllList) {
		QDir sysCacheDelAllDir(strDirPath);
		if (sysCacheDelAllDir.exists()) {
			for (QFileInfo fileInfo : sysCacheDelAllDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					//QFile::remove(strDelFilePath);
					emit updateClnSysCacheProgress(tr("删除成功:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//清理系统日志
void CleanSysAndBrwsThread::cleanLog()
{
	for (QString strDirPath : m_logDirList) {
		QDir logDir(strDirPath);
		if (logDir.exists()) {
			for (QFileInfo fileInfo : logDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					for (int i = 0; i < _countof(m_szLogSuffix); i++) {
						if (fileInfo.suffix() == QString::fromWCharArray(m_szLogSuffix[i])) {
							QString strDelFilePath = fileInfo.absoluteFilePath();
							//QFile::remove(strDelFilePath);
							emit updateClnLogProgress(tr("删除成功:%1").arg(strDelFilePath));
							QThread::msleep(30);
							break;
						}
					}
				}
			}
		}
	}
}

//清理浏览器垃圾
void CleanSysAndBrwsThread::cleanBrowserCache()
{
	for (QString strDirPath : m_brwsCacheDirList) {
		QDir brwsCacheDir(strDirPath);
		if (brwsCacheDir.exists()) {
			for (QFileInfo fileInfo : brwsCacheDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					//QFile::remove(strDelFilePath);
					emit updateClnBrowserProgress(tr("删除成功:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//临时文件后缀
TCHAR *CleanSysAndBrwsThread::m_szTmpFilesSuffix[] = {
	TEXT("tmp"), TEXT("dmp"), TEXT("log")
};

//系统缓存文件后缀
TCHAR *CleanSysAndBrwsThread::m_szSysCacheSuffix[] = {
	TEXT("dat"), TEXT("db")
};

//日志文件后缀
TCHAR *CleanSysAndBrwsThread::m_szLogSuffix[] = {
	TEXT("log"), TEXT("bak"), TEXT("txt"), TEXT("wer")
};
