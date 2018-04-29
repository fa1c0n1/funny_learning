#include "stdafx.h"
#include "AntiVirusTabWidget.h"

AntiVirusTabWidget::AntiVirusTabWidget(QWidget *parent)
	: QWidget(parent), m_pLocalScanThread(nullptr), m_pProcScanThread(nullptr)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "�������" << "״̬" << "·��";
	ui.tableWgSuspProc->setColumnCount(3);
	ui.tableWgSuspProc->setHorizontalHeaderLabels(headers);
	ui.tableWgSuspProc->setColumnWidth(0, ui.tableWgSuspProc->width() / 8 * 2);
	ui.tableWgSuspProc->setColumnWidth(1, ui.tableWgSuspProc->width() / 8);
	ui.tableWgSuspProc->setColumnWidth(2, ui.tableWgSuspProc->width() / 8 * 4);
}

AntiVirusTabWidget::~AntiVirusTabWidget()
{
}

//"ѡ��Ŀ¼"��ť�Ĳۺ���
void AntiVirusTabWidget::onSelectScanPath()
{
	QString strDirPath = QFileDialog::getExistingDirectory(NULL, tr("ѡ��Ŀ¼"), tr("."));
	ui.leScanPath->setText(strDirPath);
}

//"���ز�ɱ"��ť�Ĳۺ���
void AntiVirusTabWidget::onPsBtnLocalAVClicked()
{
	if (ui.leScanPath->text().isEmpty()) {
		QMessageBox::information(this, tr("��ʾ"), tr("ɨ��·������Ϊ��"));
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

//"�Ʋ�ɱ"��ť�ۺ���
void AntiVirusTabWidget::onPsBtnCloudAVClicked()
{
	//TODO:
}

//"���̲�ɱ"��ť�ۺ���
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

//��ȡ���̴߳��ݹ��������ݣ����½���
void AntiVirusTabWidget::onUpdateLocalScanProgress(QString strFilePath)
{
	ui.labelFilePath->setText(strFilePath);
	if (strFilePath == tr("ɨ�����")) {
		int nVirusCnt = ui.listWgVirus->count();
		if (nVirusCnt == 0) {
			ui.labelScanResult->setText(tr("����ɨ��δ���ֲ���"));
		}
		else {
			ui.labelScanResult->setText(QString::asprintf("����ɨ�蹲���� %d ������", nVirusCnt));
		}

		ui.psBtnLocalAV->setEnabled(true);
		ui.psBtnCloudAV->setEnabled(true);
		disconnect(m_pLocalScanThread, &LocalScanVirusThread::updateLocalScanProgress,
			this, &AntiVirusTabWidget::onUpdateLocalScanProgress);
		disconnect(m_pLocalScanThread, &LocalScanVirusThread::catchVirus,
			this, &AntiVirusTabWidget::onCatchVirus);
	}
}

//��ȡ���̴߳��ݹ��������ݣ����½���
void AntiVirusTabWidget::onCatchVirus(QString strFilePath)
{
	ui.listWgVirus->addItem(new QListWidgetItem(strFilePath));
}

//��ȡ���̴߳��ݹ��������ݣ����½���
void AntiVirusTabWidget::onUpdateProcScanProgress(QString strProcName)
{
	ui.labelProcess->setText(strProcName);
	if (strProcName == tr("ɨ�����")) {
		ui.psBtnProcScan->setEnabled(true);
		ui.tableWgSuspProc->setShortcutEnabled(true);
		disconnect(m_pProcScanThread, &ProcessScanVirusThread::updateProcScanProgress,
			this, &AntiVirusTabWidget::onUpdateProcScanProgress);
		disconnect(m_pProcScanThread, &ProcessScanVirusThread::catchVirusProcess,
			this, &AntiVirusTabWidget::onCatchVirusProcess);
	}
}

//��ȡ���̴߳��ݹ��������ݣ����½���
void AntiVirusTabWidget::onCatchVirusProcess(QString strProcName, QString strPath, QString strState)
{
	int nRowCnt = ui.tableWgSuspProc->rowCount();
	ui.tableWgSuspProc->setRowCount(nRowCnt + 1);
	ui.tableWgSuspProc->setItem(nRowCnt, 0, new QTableWidgetItem(strProcName));
	ui.tableWgSuspProc->setItem(nRowCnt, 1, new QTableWidgetItem(strState));
	ui.tableWgSuspProc->setItem(nRowCnt, 2, new QTableWidgetItem(strPath));
}
