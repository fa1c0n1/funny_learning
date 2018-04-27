#pragma once

#include <QWidget>
#include "ui_AntiVirusTabWidget.h"
#include "LocalScanVirusThread.h"
#include "ProcessScanVirusThread.h"


class AntiVirusTabWidget : public QWidget
{
	Q_OBJECT

public:
	AntiVirusTabWidget(QWidget *parent = Q_NULLPTR);
	~AntiVirusTabWidget();

private:
	//void localScanVirus(QString strDir);
	//void cloudScanVirus(QString strDir);
		

public slots:
void onSelectScanPath();
void onPsBtnLocalAVClicked();
void onPsBtnCloudAVClicked();
void onPsBtnProcScanClicked();
void onUpdateLocalScanProgress(QString strFilePath);
void onCatchVirus(QString strFilePath);
void onUpdateProcScanProgress(QString strProcName);
void onCatchVirusProcess(QString strProcName, QString strPath, QString strState);

private:
	Ui::AntiVirusTabWidget ui;
	LocalScanVirusThread *m_pLocalScanThread;
	ProcessScanVirusThread *m_pProcScanThread;
};
