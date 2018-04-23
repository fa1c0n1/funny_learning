/********************************************************************************
** Form generated from reading UI file 'ModuleDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODULEDLG_H
#define UI_MODULEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ModuleDlg
{
public:
    QTableWidget *tableWidgetModule;

    void setupUi(QDialog *ModuleDlg)
    {
        if (ModuleDlg->objectName().isEmpty())
            ModuleDlg->setObjectName(QStringLiteral("ModuleDlg"));
        ModuleDlg->resize(638, 429);
        tableWidgetModule = new QTableWidget(ModuleDlg);
        tableWidgetModule->setObjectName(QStringLiteral("tableWidgetModule"));
        tableWidgetModule->setGeometry(QRect(10, 10, 621, 411));
        tableWidgetModule->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidgetModule->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetModule->setSortingEnabled(true);
        tableWidgetModule->horizontalHeader()->setHighlightSections(false);
        tableWidgetModule->verticalHeader()->setVisible(false);

        retranslateUi(ModuleDlg);

        QMetaObject::connectSlotsByName(ModuleDlg);
    } // setupUi

    void retranslateUi(QDialog *ModuleDlg)
    {
        ModuleDlg->setWindowTitle(QApplication::translate("ModuleDlg", "ModuleDlg", 0));
    } // retranslateUi

};

namespace Ui {
    class ModuleDlg: public Ui_ModuleDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODULEDLG_H
