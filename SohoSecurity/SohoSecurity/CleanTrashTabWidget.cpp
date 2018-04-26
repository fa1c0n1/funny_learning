#include "stdafx.h"
#include "CleanTrashTabWidget.h"

CleanTrashTabWidget::CleanTrashTabWidget(QWidget *parent)
	: QWidget(parent), m_pCleanVSPrjThread(nullptr), m_pClnSysAndBrwsThread(nullptr)
{
	ui.setupUi(this);

	ui.textBrwVSDelFile->setFontFamily(tr("微软雅黑"));
	setAcceptDrops(true);
	
	//VS工程列表框安装事件过滤器
	ui.listWidgetVSPath->installEventFilter(this);
}

CleanTrashTabWidget::~CleanTrashTabWidget()
{
	if (m_pCleanVSPrjThread) {
		delete m_pCleanVSPrjThread;
		m_pCleanVSPrjThread = nullptr;
	}

	if (m_pClnSysAndBrwsThread) {
		delete m_pClnSysAndBrwsThread;
		m_pClnSysAndBrwsThread = nullptr;
	}
}

void CleanTrashTabWidget::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void CleanTrashTabWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty()) {
		return;
	}

	for (QUrl url : urls) {
		bool bHasAdd = false;
		QString strPath = url.toLocalFile();
		for (int i = 0; i < ui.listWidgetVSPath->count(); i++) {
			if (strPath != ui.listWidgetVSPath->item(i)->text()) {
				continue;
			}
			else {
				bHasAdd = true;
				break;
			}
		}

		if (bHasAdd)
			continue;
		else
			ui.listWidgetVSPath->addItem(strPath);
	}
}

void CleanTrashTabWidget::onPsBtnClnSysAndBrwsClicked()
{
	ui.psBtnClnSysAndBrws->setEnabled(false);
	ui.labelTmpFiles->clear();
	ui.labelSystemFiles->setText(tr("等待中"));
	ui.labelRecyBinFiles->setText(tr("等待中"));
	ui.labelSysCacheFiles->setText(tr("等待中"));
	ui.labelLogFiles->setText(tr("等待中"));
	ui.labelBrowserCache->setText(tr("等待中"));

	if (m_pClnSysAndBrwsThread == nullptr)
		m_pClnSysAndBrwsThread = new CleanSysAndBrwsThread(this);

	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnTmpFilesProgress,
		this, &CleanTrashTabWidget::onUpdateClnTmpFilesProgress);
	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnSysFilesProgress,
		this, &CleanTrashTabWidget::onUpdateClnSysFilesProgress);
	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnRecycleBinProgress,
		this, &CleanTrashTabWidget::onUpdateClnRecycleBinProgress);
	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnSysCacheProgress,
		this, &CleanTrashTabWidget::onUpdateClnSysCacheProgress);
	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnLogProgress,
		this, &CleanTrashTabWidget::onUpdateClnLogProgress);
	connect(m_pClnSysAndBrwsThread, &CleanSysAndBrwsThread::updateClnBrowserProgress,
		this, &CleanTrashTabWidget::onUpdateClnBrowserProgress);

	m_pClnSysAndBrwsThread->start();
	
}

void CleanTrashTabWidget::onpsBtnCleanVSprjClicked()
{

	ui.psBtnCleanVSprj->setEnabled(false);
	ui.textBrwVSDelFile->clear();

	if (ui.listWidgetVSPath->count() == 0) {
		ui.textBrwVSDelFile->setText(tr("没有要删除的VS工程"));
		ui.psBtnCleanVSprj->setEnabled(true);
		return;
	}

	QList<QString> prjPathList;
	for (int i = 0; i < ui.listWidgetVSPath->count(); i++) {
		QString strPrjPath = ui.listWidgetVSPath->item(i)->text();
		prjPathList.push_back(strPrjPath);
	}

	if (m_pCleanVSPrjThread == nullptr)
		m_pCleanVSPrjThread = new CleanVSPrjThread(prjPathList, this);

	connect(m_pCleanVSPrjThread, &CleanVSPrjThread::updateClnVSPrjProgress, 
		this, &CleanTrashTabWidget::onUpdateClnVSPrjProgress);
	m_pCleanVSPrjThread->start();
}

void CleanTrashTabWidget::onUpdateClnVSPrjProgress(QString strFilePath)
{
	ui.textBrwVSDelFile->append(strFilePath);
	if (strFilePath == tr("----- 清理完毕 -----")) {
		ui.psBtnCleanVSprj->setEnabled(true);
		disconnect(m_pCleanVSPrjThread, &CleanVSPrjThread::updateClnVSPrjProgress,
			this, &CleanTrashTabWidget::onUpdateClnVSPrjProgress);
	}
}

void CleanTrashTabWidget::onUpdateClnTmpFilesProgress(QString strFilePath)
{
	ui.labelTmpFiles->setText(strFilePath);
}

void CleanTrashTabWidget::onUpdateClnSysFilesProgress(QString strFilePath)
{
	ui.labelSystemFiles->setText(strFilePath);
}

void CleanTrashTabWidget::onUpdateClnRecycleBinProgress(QString strFilePath)
{
	ui.labelRecyBinFiles->setText(strFilePath);
}

void CleanTrashTabWidget::onUpdateClnSysCacheProgress(QString strFilePath)
{
	ui.labelSysCacheFiles->setText(strFilePath);
}

void CleanTrashTabWidget::onUpdateClnLogProgress(QString strFilePath)
{
	ui.labelLogFiles->setText(strFilePath);
}

void CleanTrashTabWidget::onUpdateClnBrowserProgress(QString strFilePath)
{
	ui.labelBrowserCache->setText(strFilePath);
	if (strFilePath == tr("清理完毕")) {
		ui.psBtnClnSysAndBrws->setEnabled(true);
	}
}

bool CleanTrashTabWidget::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == ui.listWidgetVSPath) {
		QKeyEvent *kEnv = static_cast<QKeyEvent *>(e);
		if (kEnv->key() == Qt::Key_Delete) {
			QList<QListWidgetItem *> pItemList = ui.listWidgetVSPath->selectedItems();
			for (QListWidgetItem *pItem : pItemList) {
				if (pItem) {
					int nRow = ui.listWidgetVSPath->row(pItem);
					ui.listWidgetVSPath->takeItem(nRow);
				}
			}
			return true;
		}
		else {
			return QWidget::eventFilter(obj, e);
		}
	}
	else {
		return QWidget::eventFilter(obj, e);
	}
}


