/********************************************************************************
** Form generated from reading UI file 'WindowTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWTABWIDGET_H
#define UI_WINDOWTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowTabWidget
{
public:
    QTableWidget *tableWidgetWnd;

    void setupUi(QWidget *WindowTabWidget)
    {
        if (WindowTabWidget->objectName().isEmpty())
            WindowTabWidget->setObjectName(QStringLiteral("WindowTabWidget"));
        WindowTabWidget->resize(942, 518);
        tableWidgetWnd = new QTableWidget(WindowTabWidget);
        tableWidgetWnd->setObjectName(QStringLiteral("tableWidgetWnd"));
        tableWidgetWnd->setGeometry(QRect(10, 10, 921, 501));
        tableWidgetWnd->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetWnd->setAlternatingRowColors(true);
        tableWidgetWnd->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetWnd->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetWnd->setSortingEnabled(true);
        tableWidgetWnd->horizontalHeader()->setHighlightSections(false);
        tableWidgetWnd->verticalHeader()->setVisible(false);

        retranslateUi(WindowTabWidget);
        QObject::connect(tableWidgetWnd, SIGNAL(customContextMenuRequested(QPoint)), WindowTabWidget, SLOT(onTableWidgetCustomContextMenuRequest(QPoint)));

        QMetaObject::connectSlotsByName(WindowTabWidget);
    } // setupUi

    void retranslateUi(QWidget *WindowTabWidget)
    {
        WindowTabWidget->setWindowTitle(QApplication::translate("WindowTabWidget", "WindowTabWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowTabWidget: public Ui_WindowTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWTABWIDGET_H
