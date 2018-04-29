#include "stdafx.h"
#include "CleanSysAndBrwsThread.h"

/************************************************************************
��ʱ�ļ���.dmp, .tmp, .log
C:/Windows
C:/Users/<�û���> [QDir::homePath()] + /AppData/Local

ϵͳ�ļ���
C:/ProgramData/Microsoft/Windows Defender/Scans
C:/ProgramData/Microsoft/Windows Defender/Definition Updates/Backup

����վ

ϵͳ���棺.dat, .db
C:/Windows/ServiceProfiles/LocalService/AppData/Local
C:/Users/<�û���> [QDir::homePath()] + /AppData/Local/Microsoft/Windows/Explorer
C:/Users/<�û���> [QDir::homePath()] + /AppData/Local

[����Ŀ¼���������ļ�]
C:/Windows/System32/config/systemprofile/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content
C:/Users/<�û���> [QDir::homePath] + /AppData/LocalLow/Microsoft/CryptnetUrlCache/Content

��־�ļ���.log, .bak, .txt, .wer
C:/Windows
C:/Windows/Logs
C:/Windows/inf
C:/Windows/System32/catroot2
C:/Windows/debug
C:/Users/fa1c0n [QDir::homePath()] + /AppData/Local/Microsoft/Windows/WER

���������:
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
	//��ʱ�ļ�
	m_tmpDirList.push_back(tr("C:/Windows"));
	m_tmpDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/Explorer"));

	//ϵͳ�ļ�
	m_sysDirList.push_back(tr("C:/ProgramData/Microsoft/Windows Defender/Scans"));
	m_sysDirList.push_back(tr("C:/ProgramData/Microsoft/Windows Defender/Definition Updates/Backup"));

	//ϵͳ����
	m_sysCacheDirList.push_back(tr("C:/Windows/ServiceProfiles/LocalService/AppData/Local"));
	m_sysCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/Explorer"));
	m_sysCacheDirList.push_back(QDir::homePath() + tr("/AppData/Local"));
	m_sysCacheDirDelAllList.push_back(tr("C:/Windows/System32/config/systemprofile/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content"));
	m_sysCacheDirDelAllList.push_back(QDir::homePath() + tr("/AppData/LocalLow/Microsoft/CryptnetUrlCache/Content"));

	//��־�ļ�
	m_logDirList.push_back(tr("C:/Windows"));
	m_logDirList.push_back(tr("C:/Windows/Logs"));
	m_logDirList.push_back(tr("C:/Windows/inf"));
	m_logDirList.push_back(tr("C:/Windows/System32/catroot2"));
	m_logDirList.push_back(tr("C:/Windows/debug"));
	m_logDirList.push_back(QDir::homePath() + tr("/AppData/Local/Microsoft/Windows/WER"));

	//���������
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
	//������ʱ�ļ�
	cleanTmpFiles();
	emit updateClnTmpFilesProgress(tr("�������"));

	//����ϵͳ�ļ�
	cleanSysFiles();
	emit updateClnSysFilesProgress(tr("�������"));

	//�������վ
	cleanRecycleBin();
	emit updateClnRecycleBinProgress(tr("�������"));

	//����ϵͳ����
	cleanSysCache();
	emit updateClnSysCacheProgress(tr("�������"));

	//������־
	cleanLog();
	emit updateClnLogProgress(tr("�������"));

	//�������������
	cleanBrowserCache();
	emit updateClnBrowserProgress(tr("�������"));
}

//������ʱ�ļ�
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
							emit updateClnTmpFilesProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
							QThread::msleep(30);
							break;
						}
					}
				}
			}
		}
	}
}

//����ϵͳ�ļ�
void CleanSysAndBrwsThread::cleanSysFiles()
{
	for (QString strDirPath : m_sysDirList) {
		QDir sysDir(strDirPath);
		if (sysDir.exists()) {
			for (QFileInfo fileInfo : sysDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					//QFile::remove(strDelFilePath);
					emit updateClnSysFilesProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//��ջ���վ
void CleanSysAndBrwsThread::cleanRecycleBin()
{
	//��ʼ�� SHQUERYRBINFO �ṹ��
	SHQUERYRBINFO recycleBinInfo = {};
	recycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

	//��ѯ����վ��Ϣ
	SHQueryRecycleBin(NULL, &recycleBinInfo);

	//��ջ���վ
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//����ϵͳ����
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
							emit updateClnSysCacheProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
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
					emit updateClnSysCacheProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//����ϵͳ��־
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
							emit updateClnLogProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
							QThread::msleep(30);
							break;
						}
					}
				}
			}
		}
	}
}

//�������������
void CleanSysAndBrwsThread::cleanBrowserCache()
{
	for (QString strDirPath : m_brwsCacheDirList) {
		QDir brwsCacheDir(strDirPath);
		if (brwsCacheDir.exists()) {
			for (QFileInfo fileInfo : brwsCacheDir.entryInfoList()) {
				if (fileInfo.isFile()) {
					QString strDelFilePath = fileInfo.absoluteFilePath();
					//QFile::remove(strDelFilePath);
					emit updateClnBrowserProgress(tr("ɾ���ɹ�:%1").arg(strDelFilePath));
					QThread::msleep(30);
				}
			}
		}
	}
}

//��ʱ�ļ���׺
TCHAR *CleanSysAndBrwsThread::m_szTmpFilesSuffix[] = {
	TEXT("tmp"), TEXT("dmp"), TEXT("log")
};

//ϵͳ�����ļ���׺
TCHAR *CleanSysAndBrwsThread::m_szSysCacheSuffix[] = {
	TEXT("dat"), TEXT("db")
};

//��־�ļ���׺
TCHAR *CleanSysAndBrwsThread::m_szLogSuffix[] = {
	TEXT("log"), TEXT("bak"), TEXT("txt"), TEXT("wer")
};
