/********************************************************************************
** Form generated from reading UI file 'SoftUninstallTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOFTUNINSTALLTABWIDGET_H
#define UI_SOFTUNINSTALLTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SoftUninstallTabWidget
{
public:
    QTableWidget *tableWgSoftMgr;

    void setupUi(QWidget *SoftUninstallTabWidget)
    {
        if (SoftUninstallTabWidget->objectName().isEmpty())
            SoftUninstallTabWidget->setObjectName(QStringLiteral("SoftUninstallTabWidget"));
        SoftUninstallTabWidget->resize(939, 509);
        tableWgSoftMgr = new QTableWidget(SoftUninstallTabWidget);
        tableWgSoftMgr->setObjectName(QStringLiteral("tableWgSoftMgr"));
        tableWgSoftMgr->setGeometry(QRect(10, 10, 921, 491));
        tableWgSoftMgr->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWgSoftMgr->setAlternatingRowColors(true);
        tableWgSoftMgr->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWgSoftMgr->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWgSoftMgr->setSortingEnabled(true);
        tableWgSoftMgr->horizontalHeader()->setHighlightSections(false);
        tableWgSoftMgr->verticalHeader()->setVisible(true);
        tableWgSoftMgr->verticalHeader()->setHighlightSections(false);

        retranslateUi(SoftUninstallTabWidget);
        QObject::connect(tableWgSoftMgr, SIGNAL(customContextMenuRequested(QPoint)), SoftUninstallTabWidget, SLOT(onTableWidgetCustomContextMenuRequest(QPoint)));

        QMetaObject::connectSlotsByName(SoftUninstallTabWidget);
    } // setupUi

    void retranslateUi(QWidget *SoftUninstallTabWidget)
    {
        SoftUninstallTabWidget->setWindowTitle(QApplication::translate("SoftUninstallTabWidget", "SoftUninstallTabWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class SoftUninstallTabWidget: public Ui_SoftUninstallTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOFTUNINSTALLTABWIDGET_H
