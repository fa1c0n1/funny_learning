/********************************************************************************
** Form generated from reading UI file 'StartItemTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTITEMTABWIDGET_H
#define UI_STARTITEMTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartItemTabWidget
{
public:
    QTableWidget *tableWgStartItemMgr;

    void setupUi(QWidget *StartItemTabWidget)
    {
        if (StartItemTabWidget->objectName().isEmpty())
            StartItemTabWidget->setObjectName(QStringLiteral("StartItemTabWidget"));
        StartItemTabWidget->resize(938, 507);
        tableWgStartItemMgr = new QTableWidget(StartItemTabWidget);
        tableWgStartItemMgr->setObjectName(QStringLiteral("tableWgStartItemMgr"));
        tableWgStartItemMgr->setGeometry(QRect(10, 10, 921, 491));

        retranslateUi(StartItemTabWidget);

        QMetaObject::connectSlotsByName(StartItemTabWidget);
    } // setupUi

    void retranslateUi(QWidget *StartItemTabWidget)
    {
        StartItemTabWidget->setWindowTitle(QApplication::translate("StartItemTabWidget", "StartItemTabWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class StartItemTabWidget: public Ui_StartItemTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTITEMTABWIDGET_H
