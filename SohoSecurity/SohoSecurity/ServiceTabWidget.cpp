#include "stdafx.h"
#include "ServiceTabWidget.h"

#pragma comment(lib, "AdvApi32.lib")

ServiceTabWidget::ServiceTabWidget(QWidget *parent)
	: QWidget(parent), m_pPopMenu(nullptr), 
	m_pActStartService(nullptr), m_pActStopService(nullptr)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "服务名" << "类型" << "状态" << "描述" <<"启动类型" << "路径";
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
		m_pActStartService = m_pPopMenu->addAction("启动服务");
		connect(m_pActStartService, &QAction::triggered, this, &ServiceTabWidget::onPopMenuStartService);
	}

	if (m_pActStopService == nullptr) {
		m_pActStopService = m_pPopMenu->addAction("停止服务");
		connect(m_pActStopService, &QAction::triggered, this, &ServiceTabWidget::onPopMenuStopService);
	}

	QString strServState = ui.tableWidgetServ->selectedItems().at(2)->text();
	if (strServState == "正在运行") {
		m_pActStartService->setEnabled(false);
		m_pActStopService->setEnabled(true);
	}
	else if (strServState == "已停止") {
		m_pActStartService->setEnabled(true);
		m_pActStopService->setEnabled(false);
	}

	m_pPopMenu->exec(QCursor::pos());
}

void ServiceTabWidget::onPopMenuRefresh()
{
//	ui.tableWidgetServ->clearContents();
//	ui.tableWidgetServ->setSortingEnabled(false); //->解决排序后刷新数据会造成控件上数据很多都为空白的bug
//	listService();
//	ui.tableWidgetServ->setSortingEnabled(true);
}

void ServiceTabWidget::onPopMenuStartService()
{
	if (startService()) {
		QTableWidgetItem *pServStateItem = ui.tableWidgetServ->selectedItems().at(2);
		pServStateItem->setText(tr("正在运行"));
	}
	else {
		QMessageBox::critical(this, tr("错误"), tr("启动服务失败，请求对该服务无效或权限不足"));
	}
}

void ServiceTabWidget::onPopMenuStopService()
{
	if (stopService()) {
		QTableWidgetItem *pServStateItem = ui.tableWidgetServ->selectedItems().at(2);
		pServStateItem->setText(tr("已停止"));
	}
	else {
		QMessageBox::critical(this, tr("错误"), tr("停止服务失败，请求对该服务无效或权限不足"));
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

	//1.打开远程计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//2. 第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32 | SERVICE_DRIVER,
		SERVICE_STATE_ALL, NULL,
		0, &dwSize, &dwServiceNum, NULL, NULL);

	//3. 申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize]{};

	//4. 第二次, 枚举
	bool bStatus = false;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32 | SERVICE_DRIVER,
		SERVICE_STATE_ALL, (PBYTE)pEnumService,
		dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	//5. 遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++) {
		//5.1 获取基础信息
		//服务名
		ui.tableWidgetServ->setRowCount(i + 1);
		ui.tableWidgetServ->setItem(i, 0, new QTableWidgetItem(QString::fromWCharArray(pEnumService[i].lpServiceName)));

		//服务类型
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			strServType = "文件系统驱动服务";
			break;
		case SERVICE_KERNEL_DRIVER:
			strServType = "设备驱动服务";
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			strServType = "独立进程服务";
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			strServType = "共享进程服务";
			break;
		default:
			strServType = "未知类型";
			break;
		}
		QTableWidgetItem *pServTypeItem = new QTableWidgetItem(strServType);
		pServTypeItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 1, pServTypeItem);

		//服务状态
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
			strServState = "即将继续";
			break;
		case SERVICE_PAUSE_PENDING:
			strServState = "正在暂停";
			break;
		case SERVICE_PAUSED:
			strServState = "已暂停";
			break;
		case SERVICE_RUNNING:
			strServState = "正在运行";
			break;
		case SERVICE_START_PENDING:
			strServState = "正在启动";
			break;
		case SERVICE_STOP_PENDING:
			strServState = "正在停止";
			break;
		case SERVICE_STOPPED:
			strServState = "已停止";
			break;
		}
		QTableWidgetItem *pServStateItem = new QTableWidgetItem(strServState);
		pServStateItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 2, pServStateItem);

		//获取描述
		ui.tableWidgetServ->setItem(i, 3, new QTableWidgetItem(QString::fromWCharArray(pEnumService[i].lpDisplayName)));

		//5.2.获取服务的详细信息
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize]{};
		//第二次调用，获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		//通过上面获取到的结构体信息具体得到想要的值
		//获取启动类型
		switch (pServiceConfig->dwStartType)
		{
		case SERVICE_AUTO_START:
			strServStartType = "自启动";
			break;
		case SERVICE_BOOT_START:
			strServStartType = "随设备加载启动(仅限驱动)";
			break;
		case SERVICE_DEMAND_START:
			strServStartType = "手动启动";
			break;
		case SERVICE_DISABLED:
			strServStartType = "已禁用";
			break;
		case SERVICE_SYSTEM_START:
			strServStartType = "随IO初始化系统启动(仅限驱动)";
			break;
		}
		QTableWidgetItem *pServStartTypeItem = new QTableWidgetItem(strServStartType);
		pServStartTypeItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidgetServ->setItem(i, 4, pServStartTypeItem);

		//获取路径信息
		ui.tableWidgetServ->setItem(i, 5, new QTableWidgetItem(QString::fromWCharArray(pServiceConfig->lpBinaryPathName)));

		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hSCM);
}

