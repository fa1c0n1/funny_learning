#include "stdafx.h"
#include "WindowTabWidget.h"

int WindowTabWidget::m_nWndCnt = 0;

WindowTabWidget::WindowTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "���ڱ���" << "���ھ��" << "��������";
	ui.tableWidgetWnd->setColumnCount(3);
	ui.tableWidgetWnd->setHorizontalHeaderLabels(headers);
	ui.tableWidgetWnd->setColumnWidth(0, ui.tableWidgetWnd->width() / 5 * 2);
	ui.tableWidgetWnd->setColumnWidth(1, ui.tableWidgetWnd->width() / 5 * 1);
	ui.tableWidgetWnd->setColumnWidth(2, ui.tableWidgetWnd->width() / 5 * 2);

	ui.tableWidgetWnd->setContextMenuPolicy(Qt::CustomContextMenu);
	listWindow();
}

WindowTabWidget::~WindowTabWidget()
{
}

void WindowTabWidget::onTableWidgetCustomContextMenuRequest(QPoint pos)
{
	QTableWidgetItem *pCurItem = ui.tableWidgetWnd->itemAt(pos);
	if (pCurItem == nullptr)
		return;

	QMenu *pPopMenu = new QMenu(ui.tableWidgetWnd);
	QAction *pActRefresh = pPopMenu->addAction("ˢ��");

	connect(pActRefresh, &QAction::triggered, this, &WindowTabWidget::onPopMenuRefresh);

	pPopMenu->exec(QCursor::pos());

	delete pActRefresh; pActRefresh = nullptr;
	delete pPopMenu; pPopMenu = nullptr;
}

void WindowTabWidget::onPopMenuRefresh()
{
	ui.tableWidgetWnd->clearContents();
	ui.tableWidgetWnd->setSortingEnabled(false); //->��������ˢ�����ݻ���ɿؼ������ݺܶ඼Ϊ�հ׵�bug
	listWindow();
	ui.tableWidgetWnd->setSortingEnabled(true);
}

void WindowTabWidget::listWindow()
{
	m_nWndCnt = 0;
	EnumWindows(&enumWndProc, (LPARAM)ui.tableWidgetWnd);
}

BOOL CALLBACK WindowTabWidget::enumWndProc(HWND hwnd, LPARAM lParam)
{
	QTableWidget *pTableWidgetWnd = (QTableWidget *)lParam;
	TCHAR szWndName[1024] = {};

	//��ȡ���ڱ���
	GetWindowText(hwnd, szWndName, _countof(szWndName));
	if (IsWindowVisible(hwnd) == TRUE && wcslen(szWndName) > 0) {
		//����û�б������Ҵ��ڱ��ⳤ�Ȳ�Ϊ0, ����ʾ������Ϣ
		TCHAR szWndClassName[1024] = { 0 };

		//��ȡ���ڵ�����
		GetClassName(hwnd, szWndClassName, _countof(szWndClassName));
		
		pTableWidgetWnd->setRowCount(m_nWndCnt + 1);
		//��ʽ�����ھ��
		pTableWidgetWnd->setItem(m_nWndCnt, 0, new QTableWidgetItem(QString::fromWCharArray(szWndName)));
		QTableWidgetItem *pHwndItem = new QTableWidgetItem(QString::asprintf("0x%016X", hwnd));
		pHwndItem->setTextAlignment(Qt::AlignCenter);
		pTableWidgetWnd->setItem(m_nWndCnt, 1, pHwndItem);
		pTableWidgetWnd->setItem(m_nWndCnt, 2, new QTableWidgetItem(QString::fromWCharArray(szWndClassName)));
		m_nWndCnt++;
	}

	return TRUE;
}