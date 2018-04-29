/********************************************************************************
** Form generated from reading UI file 'ThreadDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREADDLG_H
#define UI_THREADDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ThreadDlg
{
public:
    QTableWidget *tableWidgetThread;

    void setupUi(QDialog *ThreadDlg)
    {
        if (ThreadDlg->objectName().isEmpty())
            ThreadDlg->setObjectName(QStringLiteral("ThreadDlg"));
        ThreadDlg->resize(640, 426);
        tableWidgetThread = new QTableWidget(ThreadDlg);
        tableWidgetThread->setObjectName(QStringLiteral("tableWidgetThread"));
        tableWidgetThread->setGeometry(QRect(10, 10, 621, 411));
        tableWidgetThread->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetThread->setAlternatingRowColors(true);
        tableWidgetThread->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetThread->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetThread->setSortingEnabled(true);
        tableWidgetThread->horizontalHeader()->setHighlightSections(false);
        tableWidgetThread->verticalHeader()->setVisible(false);

        retranslateUi(ThreadDlg);
        QObject::connect(tableWidgetThread, SIGNAL(customContextMenuRequested(QPoint)), ThreadDlg, SLOT(onTableWidgetCustomContextMenuRequest(QPoint)));

        QMetaObject::connectSlotsByName(ThreadDlg);
    } // setupUi

    void retranslateUi(QDialog *ThreadDlg)
    {
        ThreadDlg->setWindowTitle(QApplication::translate("ThreadDlg", "ThreadDlg", 0));
    } // retranslateUi

};

namespace Ui {
    class ThreadDlg: public Ui_ThreadDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREADDLG_H
