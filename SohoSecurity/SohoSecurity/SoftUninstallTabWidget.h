#pragma once

#include <QWidget>
#include "ui_SoftUninstallTabWidget.h"

typedef struct _SOFTINFO {
	DWORD dwSoftSize;                 //�����С
	TCHAR szSoftName[64];            //�������
	TCHAR szSoftVer[64];             //����汾��
	TCHAR szSoftDate[32];            //�����װ����
	TCHAR szSoftInsPath[MAX_PATH];   //�����װ·��
	TCHAR szSoftUnsPath[MAX_PATH];   //���ж��·��
	TCHAR szSoftVenRel[64];          //�����������
	TCHAR szSoftIco[MAX_PATH];       //���ͼ��·��
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
	QVector<SOFTINFO> m_vtSoftInfo;        //������������Ϣ
};
