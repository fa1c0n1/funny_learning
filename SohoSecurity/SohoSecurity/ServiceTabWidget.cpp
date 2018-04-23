#include "stdafx.h"
#include "ServiceTabWidget.h"

#pragma comment(lib, "AdvApi32.lib")

ServiceTabWidget::ServiceTabWidget(QWidget *parent)
	: QWidget(parent), m_pPopMenu(nullptr), 
	m_pActStartService(nullptr), m_pActStopService(nullptr)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "������" << "����" << "״̬" << "����" <<"��������" << "·��";
	ui.tableWidgetServ->setColumnCount(6);
	ui.tableWidgetServ->setHorizontalHeaderLabels(headers);
	ui.tableWidgetServ->setColumnWidth(0, ui.tableWidgetServ->width() / 6);
	ui.tableWidgetServ->setColumnWidth(1, ui.tableWidgetServ->width() / 6);
	ui.tableWidgetServ->setColumnWidth(2, ui.tableWidgetServ->width() / 6);
	ui.tableWidgetServ->setColumnWidth(3, ui.tableWidgetServ->width() / 6);
	ui.tableWidgetServ->setColumnWidth(4, ui.tableWidgetServ->width() / 6);
	ui.tableWidgetServ->setColumnWidth(5, ui.tableWidgetServ->width() / 6 * 3);

	ui.tableWidgetServ->setContextMenuPolicy(Qt::CustomContextMenu);
	listService();
}

ServiceTabWidget::~ServiceTabWidget()
{
	if (m_pActStartService) {
		delete m_pActStartService;
		m_pActStartService = nullptr;
	}

	if (m_pActStopService) {
		delete m_pActStopService;
		m_pActStopService = nullptr;
	}

	if (m_pPopMenu) {
		delete m_pPopMenu;
		m_pPopMenu = nullptr;
	}
}

void ServiceTabWidget::onTableWidgetCustomContextMenuRequest(QPoint pos)
{
	QTableWidgetItem *pCurItem = ui.tableWidgetServ->itemAt(pos);
	if (pCurItem == nullptr)
		return;

	if (m_pPopMenu == nullptr)
		m_pPopMenu = new QMenu(ui.tableWidgetServ);

	if (m_pActStartService == nullptr) {
		m_pActStartService = m_pPopMenu->addAction("��������");
		connect(m_pActStartService, &QAction::triggered, this, &ServiceTabWidget::onPopMenuStartService);
	}

	if (m_pActStopService == nullptr) {
		m_pActStopService = m_pPopMenu->addAction("ֹͣ����");
		connect(m_pActStopService, &QAction::triggered, this, &ServiceTabWidget::onPopMenuStopService);
	}

	QString strServState = ui.tableWidgetServ->selectedItems().at(2)->text();
	if (strServState == "��������") {
		m_pActStartService->setEnabled(false);
		m_pActStopService->setEnabled(true);
	}
	else if (strServState == "��ֹͣ") {
		m_pActStartService->setEnabled(true);
		m_pActStopService->setEnabled(false);
	}

	m_pPopMenu->exec(QCursor::pos());
}

void ServiceTabWidget::onPopMenuRefresh()
{
//	ui.tableWidgetServ->clearContents();
//	ui.tableWidgetServ->setSortingEnabled(false); //->��������ˢ�����ݻ���ɿؼ������ݺܶ඼Ϊ�հ׵�bug
//	listService();
//	ui.tableWidgetServ->setSortingEnabled(true);
}

void ServiceTabWidget::onPopMenuStartService()
{
	if (startService()) {
		QTableWidgetItem *pServStateItem = ui.tableWidgetServ->selectedItems().at(2);
		pServStateItem->setText(tr("��������"));
	}
	else {
		QMessageBox::critical(this, tr("����"), tr("��������ʧ�ܣ�����Ը÷�����Ч��Ȩ�޲���"));
	}
}

void ServiceTabWidget::onPopMenuStopService()
{
	if (stopService()) {
		QTableWidgetItem *pServStateItem = ui.tableWidgetServ->selectedItems().at(2);
		pServStateItem->setText(tr("��ֹͣ"));
	}
	else {
		QMessageBox::critical(this, tr("����"), tr("ֹͣ����ʧ�ܣ�����Ը÷�����Ч��Ȩ�޲���"));
	}
}

bool ServiceTabWidget::startService()
{
	bool bRet = false;
	QTableWidgetItem *pServNameItem = ui.tableWidgetServ->selectedItems().at(0);
	QString strServName = pServNameItem->text();
	TCHAR szServName[MAX_PATH] = {};

	strServName.toWCharArray(szServName);
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM, szServName, SERVICE_START);
	if (hService)
		bRet = StartService(hService, 0, NULL) != 0;

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	return bRet;
}

bool ServiceTabWidget::stopService()
{
	bool bRet = false;
	QTableWidgetItem *pServNameItem = ui.tableWidgetServ->selectedItems().at(0);
	QString strServName = pServNameItem->text();
	TCHAR szServName[MAX_PATH] = {};

	strServName.toWCharArray(szServName);
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM, szServName, SERVICE_STOP);
	if (hService) {
		SERVICE_STATUS servStatus = { 0 };
		bRet = ControlService(hService, SERVICE_CONTROL_STOP, &servStatus) != 0;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);

	return bRet;
}

void ServiceTabWidget::listService()
{
	QString strServType;
	QString strServState;
	QString strServStartType;

	//1.��Զ�̼����������ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//2. ��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32 | SERVICE_DRIVER,
		SERVICE_STATE_ALL, NULL,
		0, &dwSize, &dwServiceNum, NULL, NULL);

	//3. ������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize]{};

	//4. �ڶ���, ö��
	bool bStatus = false;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32 | SERVICE_DRIVER,
		SERVICE_STATE_ALL, (PBYTE)pEnumService,
		dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//5. ������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++) {
		//5.1 ��ȡ������Ϣ
		//������
		ui.tableWidgetServ->setRowCount(i + 1);
		ui.tableWidgetServ->setItem(i, 0, new QTableWidgetItem(QString::fromWCharArray(pEnumService[i].lpServiceName)));

		//��������
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			strServType = "�ļ�ϵͳ��������";
			break;
		case SERVICE_KERNEL_DRIVER:
			strServType = "�豸��������";
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			strServType = "�������̷���";
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			strServType = "������̷���";
			break;
		default:
			strServType = "δ֪����";
			break;
		}
		QTableWidgetItem *pServTypeItem = new QTableWidgetItem(strServType);
		pServTypeItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 1, pServTypeItem);

		//����״̬
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
			strServState = "��������";
			break;
		case SERVICE_PAUSE_PENDING:
			strServState = "������ͣ";
			break;
		case SERVICE_PAUSED:
			strServState = "����ͣ";
			break;
		case SERVICE_RUNNING:
			strServState = "��������";
			break;
		case SERVICE_START_PENDING:
			strServState = "��������";
			break;
		case SERVICE_STOP_PENDING:
			strServState = "����ֹͣ";
			break;
		case SERVICE_STOPPED:
			strServState = "��ֹͣ";
			break;
		}
		QTableWidgetItem *pServStateItem = new QTableWidgetItem(strServState);
		pServStateItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 2, pServStateItem);

		//��ȡ����
		ui.tableWidgetServ->setItem(i, 3, new QTableWidgetItem(QString::fromWCharArray(pEnumService[i].lpDisplayName)));

		//5.2.��ȡ�������ϸ��Ϣ
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize]{};
		//�ڶ��ε��ã���ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		//ͨ�������ȡ���Ľṹ����Ϣ����õ���Ҫ��ֵ
		//��ȡ��������
		switch (pServiceConfig->dwStartType)
		{
		case SERVICE_AUTO_START:
			strServStartType = "������";
			break;
		case SERVICE_BOOT_START:
			strServStartType = "���豸��������(��������)";
			break;
		case SERVICE_DEMAND_START:
			strServStartType = "�ֶ�����";
			break;
		case SERVICE_DISABLED:
			strServStartType = "�ѽ���";
			break;
		case SERVICE_SYSTEM_START:
			strServStartType = "��IO��ʼ��ϵͳ����(��������)";
			break;
		}
		QTableWidgetItem *pServStartTypeItem = new QTableWidgetItem(strServStartType);
		pServStartTypeItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 4, pServStartTypeItem);

		//��ȡ·����Ϣ
		ui.tableWidgetServ->setItem(i, 5, new QTableWidgetItem(QString::fromWCharArray(pServiceConfig->lpBinaryPathName)));

		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
}

