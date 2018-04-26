#pragma once

#include <QWidget>
#include "CleanVSPrjThread.h"
#include "CleanSysAndBrwsThread.h"
#include "ui_CleanTrashTabWidget.h"

class CleanTrashTabWidget : public QWidget
{
	Q_OBJECT

public:
	CleanTrashTabWidget(QWidget *parent = Q_NULLPTR);
	~CleanTrashTabWidget();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	//ÊÂ¼þ¹ýÂËÆ÷
	bool eventFilter(QObject *obj, QEvent *e);

public slots:
void onPsBtnClnSysAndBrwsClicked();
void onpsBtnCleanVSprjClicked();
void onUpdateClnVSPrjProgress(QString strFilePath);
void onUpdateClnTmpFilesProgress(QString strFilePath);
void onUpdateClnSysFilesProgress(QString strFilePath);
void onUpdateClnRecycleBinProgress(QString strFilePath);
void onUpdateClnSysCacheProgress(QString strFilePath);
void onUpdateClnLogProgress(QString strFilePath);
void onUpdateClnBrowserProgress(QString strFilePath);


private:
	Ui::CleanTrashTabWidget ui;
	CleanVSPrjThread *m_pCleanVSPrjThread;
	CleanSysAndBrwsThread *m_pClnSysAndBrwsThread;
};
