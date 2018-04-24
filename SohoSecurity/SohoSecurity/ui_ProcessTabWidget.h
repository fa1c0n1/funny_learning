/********************************************************************************
** Form generated from reading UI file 'ProcessTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSTABWIDGET_H
#define UI_PROCESSTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProcessTabWidget
{
public:
    QTableWidget *tableWidget;

    void setupUi(QWidget *ProcessTabWidget)
    {
        if (ProcessTabWidget->objectName().isEmpty())
            ProcessTabWidget->setObjectName(QStringLiteral("ProcessTabWidget"));
        ProcessTabWidget->resize(939, 518);
        tableWidget = new QTableWidget(ProcessTabWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 921, 501));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setSortingEnabled(true);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setVisible(false);

        retranslateUi(ProcessTabWidget);
        QObject::connect(tableWidget, SIGNAL(customContextMenuRequested(QPoint)), ProcessTabWidget, SLOT(onTableWidgetCustomContextMenuRequest(QPoint)));

        QMetaObject::connectSlotsByName(ProcessTabWidget);
    } // setupUi

    void retranslateUi(QWidget *ProcessTabWidget)
    {
        ProcessTabWidget->setWindowTitle(QApplication::translate("ProcessTabWidget", "ProcessTabWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class ProcessTabWidget: public Ui_ProcessTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSTABWIDGET_H
