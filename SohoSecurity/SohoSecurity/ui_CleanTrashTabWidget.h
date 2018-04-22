/********************************************************************************
** Form generated from reading UI file 'CleanTrashTabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLEANTRASHTABWIDGET_H
#define UI_CLEANTRASHTABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CleanTrashTabWidget
{
public:
    QLabel *label;

    void setupUi(QWidget *CleanTrashTabWidget)
    {
        if (CleanTrashTabWidget->objectName().isEmpty())
            CleanTrashTabWidget->setObjectName(QStringLiteral("CleanTrashTabWidget"));
        CleanTrashTabWidget->resize(862, 279);
        label = new QLabel(CleanTrashTabWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 90, 91, 21));

        retranslateUi(CleanTrashTabWidget);

        QMetaObject::connectSlotsByName(CleanTrashTabWidget);
    } // setupUi

    void retranslateUi(QWidget *CleanTrashTabWidget)
    {
        CleanTrashTabWidget->setWindowTitle(QApplication::translate("CleanTrashTabWidget", "CleanTrashTabWidget", 0));
        label->setText(QApplication::translate("CleanTrashTabWidget", "\345\236\203\345\234\276\346\270\205\347\220\206TAB", 0));
    } // retranslateUi

};

namespace Ui {
    class CleanTrashTabWidget: public Ui_CleanTrashTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLEANTRASHTABWIDGET_H
