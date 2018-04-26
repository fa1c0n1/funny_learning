#pragma once

#include <QWidget>
#include "ui_SoftUninstallTabWidget.h"

typedef struct _SOFTINFO {
	DWORD dwSoftSize;                 //软件大小
	TCHAR szSoftName[64];            //软件名称
	TCHAR szSoftVer[64];             //软件版本号
	TCHAR szSoftDate[32];            //软件安装日期
	TCHAR szSoftInsPath[MAX_PATH];   //软件安装路径
	TCHAR szSoftUnsPath[MAX_PATH];   //软件卸载路径
	TCHAR szSoftVenRel[64];          //软件发布厂商
	TCHAR szSoftIco[MAX_PATH];       //软件图标路径
} SOFTINFO, *PSOFTINFO;

class SoftUninstallTabWidget : public QWidget
{
	Q_OBJECT

public:
	SoftUninstallTabWidget(QWidget *parent = Q_NULLPTR);
	~SoftUninstallTabWidget();

protected:
	void timerEvent(QTimerEvent *tEvent);

private:
	void listSoftWare();

public slots :
void onTableWidgetCustomContextMenuRequest(QPoint pos);
void onUninstallSoft();

private:
	Ui::SoftUninstallTabWidget ui;
	SOFTINFO m_softInfo;
	int m_nTimerId;
	QVector<SOFTINFO> m_vtSoftInfo;        //保存软件相关信息
};
