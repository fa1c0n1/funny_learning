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
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceTabWidget
{
public:
    QLabel *label;

    void setupUi(QWidget *ServiceTabWidget)
    {
        if (ServiceTabWidget->objectName().isEmpty())
            ServiceTabWidget->setObjectName(QStringLiteral("ServiceTabWidget"));
        ServiceTabWidget->resize(872, 411);
        label = new QLabel(ServiceTabWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(190, 90, 81, 21));

        retranslateUi(ServiceTabWidget);

        QMetaObject::connectSlotsByName(ServiceTabWidget);
    } // setupUi

    void retranslateUi(QWidget *ServiceTabWidget)
    {
        ServiceTabWidget->setWindowTitle(QApplication::translate("ServiceTabWidget", "ServiceTabWidget", 0));
        label->setText(QApplication::translate("ServiceTabWidget", "\346\234\215\345\212\241TAB", 0));
    } // retranslateUi

};

namespace Ui {
    class ServiceTabWidget: public Ui_ServiceTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICETABWIDGET_H
