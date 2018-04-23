/********************************************************************************
** Form generated from reading UI file 'AntiVirusTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANTIVIRUSTABWIDGET_H
#define UI_ANTIVIRUSTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AntiVirusTabWidget
{
public:
    QLabel *label;

    void setupUi(QWidget *AntiVirusTabWidget)
    {
        if (AntiVirusTabWidget->objectName().isEmpty())
            AntiVirusTabWidget->setObjectName(QStringLiteral("AntiVirusTabWidget"));
        AntiVirusTabWidget->resize(868, 332);
        label = new QLabel(AntiVirusTabWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(170, 100, 121, 21));

        retranslateUi(AntiVirusTabWidget);

        QMetaObject::connectSlotsByName(AntiVirusTabWidget);
    } // setupUi

    void retranslateUi(QWidget *AntiVirusTabWidget)
    {
        AntiVirusTabWidget->setWindowTitle(QApplication::translate("AntiVirusTabWidget", "AntiVirusTabWidget", 0));
        label->setText(QApplication::translate("AntiVirusTabWidget", "\347\227\205\346\257\222\346\237\245\346\235\200TAB", 0));
    } // retranslateUi

};

namespace Ui {
    class AntiVirusTabWidget: public Ui_AntiVirusTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANTIVIRUSTABWIDGET_H
