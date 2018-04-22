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
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowTabWidget
{
public:
    QLabel *label;

    void setupUi(QWidget *WindowTabWidget)
    {
        if (WindowTabWidget->objectName().isEmpty())
            WindowTabWidget->setObjectName(QStringLiteral("WindowTabWidget"));
        WindowTabWidget->resize(874, 438);
        label = new QLabel(WindowTabWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 80, 101, 21));

        retranslateUi(WindowTabWidget);

        QMetaObject::connectSlotsByName(WindowTabWidget);
    } // setupUi

    void retranslateUi(QWidget *WindowTabWidget)
    {
        WindowTabWidget->setWindowTitle(QApplication::translate("WindowTabWidget", "WindowTabWidget", 0));
        label->setText(QApplication::translate("WindowTabWidget", "\347\252\227\345\217\243TAB", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowTabWidget: public Ui_WindowTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWTABWIDGET_H
