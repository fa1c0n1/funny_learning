#include "stdafx.h"
#include "ServiceTabWidget.h"

ServiceTabWidget::ServiceTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QSplitter *pSplitter = new QSplitter(this);

	QFileSystemModel *pModel = new QFileSystemModel;
	pModel->setRootPath(tr("C:/Users/fa1c0n/Desktop"));

	QTreeView *pTree = new QTreeView(pSplitter);
	pTree->setModel(pModel);
	pTree->setRootIndex(pModel->index(QDir::homePath()));
	//pTree->showColumn(0);
	pTree->setColumnHidden(1, true);
	pTree->setColumnHidden(2, true);
	pTree->setColumnHidden(3, true);

	/*QListView *pList = new QListView(pSplitter);
	pList->setModel(pModel);
	pList->setRootIndex(pModel->index(QDir::currentPath()));*/

	QVBoxLayout *pLayout = new QVBoxLayout;
	pLayout->addWidget(pSplitter);
	setLayout(pLayout);
}

ServiceTabWidget::~ServiceTabWidget()
{
}
