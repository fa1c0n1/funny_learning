/********************************************************************************
** Form generated from reading UI file 'ServiceTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICETABWIDGET_H
#define UI_SERVICETABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceTabWidget
{
public:
    QTableWidget *tableWidgetServ;

    void setupUi(QWidget *ServiceTabWidget)
    {
        if (ServiceTabWidget->objectName().isEmpty())
            ServiceTabWidget->setObjectName(QStringLiteral("ServiceTabWidget"));
        ServiceTabWidget->resize(939, 515);
        tableWidgetServ = new QTableWidget(ServiceTabWidget);
        tableWidgetServ->setObjectName(QStringLiteral("tableWidgetServ"));
        tableWidgetServ->setGeometry(QRect(10, 10, 921, 501));
        tableWidgetServ->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetServ->setAlternatingRowColors(true);
        tableWidgetServ->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetServ->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetServ->setSortingEnabled(true);
        tableWidgetServ->horizontalHeader()->setHighlightSections(false);
        tableWidgetServ->verticalHeader()->setVisible(false);

        retranslateUi(ServiceTabWidget);
        QObject::connect(tableWidgetServ, SIGNAL(customContextMenuRequested(QPoint)), ServiceTabWidget, SLOT(onTableWidgetCustomContextMenuRequest(QPoint)));

        QMetaObject::connectSlotsByName(ServiceTabWidget);
    } // setupUi

    void retranslateUi(QWidget *ServiceTabWidget)
    {
        ServiceTabWidget->setWindowTitle(QApplication::translate("ServiceTabWidget", "ServiceTabWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class ServiceTabWidget: public Ui_ServiceTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICETABWIDGET_H
