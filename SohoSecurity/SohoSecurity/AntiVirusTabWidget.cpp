#include "stdafx.h"
#include "AntiVirusTabWidget.h"

AntiVirusTabWidget::AntiVirusTabWidget(QWidget *parent)
	: QWidget(parent), m_pLocalScanThread(nullptr), m_pProcScanThread(nullptr)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "恶意进程" << "状态" << "路径";
	ui.tableWgSuspProc->setColumnCount(3);
	ui.tableWgSuspProc->setHorizontalHeaderLabels(headers);
	ui.tableWgSuspProc->setColumnWidth(0, ui.tableWgSuspProc->width() / 8 * 2);
	ui.tableWgSuspProc->setColumnWidth(1, ui.tableWgSuspProc->width() / 8);
	ui.tableWgSuspProc->setColumnWidth(2, ui.tableWgSuspProc->width() / 8 * 4);
}

AntiVirusTabWidget::~AntiVirusTabWidget()
{
}

//"选择目录"按钮的槽函数
void AntiVirusTabWidget::onSelectScanPath()
{
	QString strDirPath = QFileDialog::getExistingDirectory(NULL, tr("选择目录"), tr("."));
	ui.leScanPath->setText(strDirPath);
}

//"本地查杀"按钮的槽函数
void AntiVirusTabWidget::onPsBtnLocalAVClicked()
{
	if (ui.leScanPath->text().isEmpty()) {
		QMessageBox::information(this, tr("提示"), tr("扫描路径不能为空"));
		return;
	}

	ui.psBtnLocalAV->setEnabled(false);
	ui.psBtnCloudAV->setEnabled(false);
	ui.listWgVirus->clear();
	ui.labelScanResult->clear();

	if (m_pLocalScanThread == nullptr)
		m_pLocalScanThread = new LocalScanVirusThread(this);

	m_pLocalScanThread->setScanDir(ui.leScanPath->text());

	connect(m_pLocalScanThread, &LocalScanVirusThread::updateLocalScanProgress,
		this, &AntiVirusTabWidget::onUpdateLocalScanProgress);
	connect(m_pLocalScanThread, &LocalScanVirusThread::catchVirus,
		this, &AntiVirusTabWidget::onCatchVirus);

	m_pLocalScanThread->start();
}

//"云查杀"按钮槽函数
void AntiVirusTabWidget::onPsBtnCloudAVClicked()
{
	//TODO:
}

//"进程查杀"按钮槽函数
void AntiVirusTabWidget::onPsBtnProcScanClicked()
{
	ui.psBtnProcScan->setEnabled(false);
	ui.tableWgSuspProc->setShortcutEnabled(false);
	ui.tableWgSuspProc->clearContents();
	ui.tableWgSuspProc->setRowCount(0);
	ui.labelProcess->clear();

	if (m_pProcScanThread == nullptr)
		m_pProcScanThread = new ProcessScanVirusThread(this);

	connect(m_pProcScanThread, &ProcessScanVirusThread::updateProcScanProgress,
		this, &AntiVirusTabWidget::onUpdateProcScanProgress);
	connect(m_pProcScanThread, &ProcessScanVirusThread::catchVirusProcess,
		this, &AntiVirusTabWidget::onCatchVirusProcess);

	m_pProcScanThread->start();

}

//获取子线程传递过来的数据，更新界面
void AntiVirusTabWidget::onUpdateLocalScanProgress(QString strFilePath)
{
	ui.labelFilePath->setText(strFilePath);
	if (strFilePath == tr("扫描完毕")) {
		int nVirusCnt = ui.listWgVirus->count();
		if (nVirusCnt == 0) {
			ui.labelScanResult->setText(tr("本次扫描未发现病毒"));
		}
		else {
			ui.labelScanResult->setText(QString::asprintf("本次扫描共发现 %d 个病毒", nVirusCnt));
		}

		ui.psBtnLocalAV->setEnabled(true);
		ui.psBtnCloudAV->setEnabled(true);
		disconnect(m_pLocalScanThread, &LocalScanVirusThread::updateLocalScanProgress,
			this, &AntiVirusTabWidget::onUpdateLocalScanProgress);
		disconnect(m_pLocalScanThread, &LocalScanVirusThread::catchVirus,
			this, &AntiVirusTabWidget::onCatchVirus);
	}
}

//获取子线程传递过来的数据，更新界面
void AntiVirusTabWidget::onCatchVirus(QString strFilePath)
{
	ui.listWgVirus->addItem(new QListWidgetItem(strFilePath));
}

//获取子线程传递过来的数据，更新界面
void AntiVirusTabWidget::onUpdateProcScanProgress(QString strProcName)
{
	ui.labelProcess->setText(strProcName);
	if (strProcName == tr("扫描完毕")) {
		ui.psBtnProcScan->setEnabled(true);
		ui.tableWgSuspProc->setShortcutEnabled(true);
		disconnect(m_pProcScanThread, &ProcessScanVirusThread::updateProcScanProgress,
			this, &AntiVirusTabWidget::onUpdateProcScanProgress);
		disconnect(m_pProcScanThread, &ProcessScanVirusThread::catchVirusProcess,
			this, &AntiVirusTabWidget::onCatchVirusProcess);
	}
}

//获取子线程传递过来的数据，更新界面
void AntiVirusTabWidget::onCatchVirusProcess(QString strProcName, QString strPath, QString strState)
{
	int nRowCnt = ui.tableWgSuspProc->rowCount();
	ui.tableWgSuspProc->setRowCount(nRowCnt + 1);
	ui.tableWgSuspProc->setItem(nRowCnt, 0, new QTableWidgetItem(strProcName));
	ui.tableWgSuspProc->setItem(nRowCnt, 1, new QTableWidgetItem(strState));
	ui.tableWgSuspProc->setItem(nRowCnt, 2, new QTableWidgetItem(strPath));
}
