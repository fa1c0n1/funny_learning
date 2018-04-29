/********************************************************************************
** Form generated from reading UI file 'PETabWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PETABWIDGET_H
#define UI_PETABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PETabWidget
{
public:
    QPushButton *psBtnSectionTable;
    QPushButton *psBtnDataDirTable;
    QPushButton *psBtnImpTable;
    QPushButton *psBtnExpTable;
    QPushButton *psBtnResTable;
    QPushButton *psBtnRelocTable;
    QPushButton *psBtnDelayLoadTable;
    QGroupBox *groupBox;
    QTextBrowser *textBrwFileInfo;
    QGroupBox *groupBox_2;
    QTextBrowser *textBrwPEHeader;
    QGroupBox *groupBoxPEData;
    QTextBrowser *textBrwPEData;
    QPushButton *psBtnOffsetCounter;
    QPushButton *psBtnTLSTable;

    void setupUi(QWidget *PETabWidget)
    {
        if (PETabWidget->objectName().isEmpty())
            PETabWidget->setObjectName(QStringLiteral("PETabWidget"));
        PETabWidget->resize(939, 506);
        psBtnSectionTable = new QPushButton(PETabWidget);
        psBtnSectionTable->setObjectName(QStringLiteral("psBtnSectionTable"));
        psBtnSectionTable->setGeometry(QRect(850, 90, 75, 23));
        psBtnDataDirTable = new QPushButton(PETabWidget);
        psBtnDataDirTable->setObjectName(QStringLiteral("psBtnDataDirTable"));
        psBtnDataDirTable->setGeometry(QRect(850, 120, 75, 23));
        psBtnImpTable = new QPushButton(PETabWidget);
        psBtnImpTable->setObjectName(QStringLiteral("psBtnImpTable"));
        psBtnImpTable->setGeometry(QRect(850, 150, 75, 23));
        psBtnExpTable = new QPushButton(PETabWidget);
        psBtnExpTable->setObjectName(QStringLiteral("psBtnExpTable"));
        psBtnExpTable->setGeometry(QRect(850, 180, 75, 23));
        psBtnResTable = new QPushButton(PETabWidget);
        psBtnResTable->setObjectName(QStringLiteral("psBtnResTable"));
        psBtnResTable->setGeometry(QRect(850, 210, 75, 23));
        psBtnRelocTable = new QPushButton(PETabWidget);
        psBtnRelocTable->setObjectName(QStringLiteral("psBtnRelocTable"));
        psBtnRelocTable->setGeometry(QRect(850, 240, 75, 23));
        psBtnDelayLoadTable = new QPushButton(PETabWidget);
        psBtnDelayLoadTable->setObjectName(QStringLiteral("psBtnDelayLoadTable"));
        psBtnDelayLoadTable->setGeometry(QRect(850, 300, 75, 23));
        groupBox = new QGroupBox(PETabWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 401, 161));
        textBrwFileInfo = new QTextBrowser(groupBox);
        textBrwFileInfo->setObjectName(QStringLiteral("textBrwFileInfo"));
        textBrwFileInfo->setGeometry(QRect(10, 20, 381, 131));
        groupBox_2 = new QGroupBox(PETabWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 180, 401, 321));
        textBrwPEHeader = new QTextBrowser(groupBox_2);
        textBrwPEHeader->setObjectName(QStringLiteral("textBrwPEHeader"));
        textBrwPEHeader->setGeometry(QRect(10, 20, 381, 291));
        groupBoxPEData = new QGroupBox(PETabWidget);
        groupBoxPEData->setObjectName(QStringLiteral("groupBoxPEData"));
        groupBoxPEData->setGeometry(QRect(420, 10, 421, 491));
        textBrwPEData = new QTextBrowser(groupBoxPEData);
        textBrwPEData->setObjectName(QStringLiteral("textBrwPEData"));
        textBrwPEData->setGeometry(QRect(10, 20, 401, 461));
        psBtnOffsetCounter = new QPushButton(PETabWidget);
        psBtnOffsetCounter->setObjectName(QStringLiteral("psBtnOffsetCounter"));
        psBtnOffsetCounter->setGeometry(QRect(850, 330, 75, 23));
        psBtnTLSTable = new QPushButton(PETabWidget);
        psBtnTLSTable->setObjectName(QStringLiteral("psBtnTLSTable"));
        psBtnTLSTable->setGeometry(QRect(850, 270, 75, 23));

        retranslateUi(PETabWidget);
        QObject::connect(psBtnSectionTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnSectionTableClicked()));
        QObject::connect(psBtnDataDirTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnDataDirTableClicked()));
        QObject::connect(psBtnImpTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnImpTableClicked()));
        QObject::connect(psBtnExpTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnExpTableClicked()));
        QObject::connect(psBtnResTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnResTableClicked()));
        QObject::connect(psBtnRelocTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnRelocTableClicked()));
        QObject::connect(psBtnDelayLoadTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnDelayTableClicked()));
        QObject::connect(psBtnOffsetCounter, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnOffsetCounterClicked()));
        QObject::connect(psBtnTLSTable, SIGNAL(clicked()), PETabWidget, SLOT(onPsBtnTLSTableClicked()));

        QMetaObject::connectSlotsByName(PETabWidget);
    } // setupUi

    void retranslateUi(QWidget *PETabWidget)
    {
        PETabWidget->setWindowTitle(QApplication::translate("PETabWidget", "PETabWidget", 0));
        psBtnSectionTable->setText(QApplication::translate("PETabWidget", "\345\214\272\346\256\265\350\241\250", 0));
        psBtnDataDirTable->setText(QApplication::translate("PETabWidget", "\346\225\260\346\215\256\347\233\256\345\275\225\350\241\250", 0));
        psBtnImpTable->setText(QApplication::translate("PETabWidget", "\345\257\274\345\205\245\350\241\250", 0));
        psBtnExpTable->setText(QApplication::translate("PETabWidget", "\345\257\274\345\207\272\350\241\250", 0));
        psBtnResTable->setText(QApplication::translate("PETabWidget", "\350\265\204\346\272\220\350\241\250", 0));
        psBtnRelocTable->setText(QApplication::translate("PETabWidget", "\351\207\215\345\256\232\344\275\215\350\241\250", 0));
        psBtnDelayLoadTable->setText(QApplication::translate("PETabWidget", "\345\273\266\350\277\237\345\212\240\350\275\275\350\241\250", 0));
        groupBox->setTitle(QApplication::translate("PETabWidget", "\346\226\207\344\273\266\344\277\241\346\201\257", 0));
        groupBox_2->setTitle(QApplication::translate("PETabWidget", "PE\345\244\264\344\277\241\346\201\257", 0));
        groupBoxPEData->setTitle(QApplication::translate("PETabWidget", "PE\346\225\260\346\215\256\346\230\276\347\244\272\345\214\272\345\237\237", 0));
        psBtnOffsetCounter->setText(QApplication::translate("PETabWidget", "\344\275\215\347\275\256\350\256\241\347\256\227\345\231\250", 0));
        psBtnTLSTable->setText(QApplication::translate("PETabWidget", "TLS\350\241\250", 0));
    } // retranslateUi

};

namespace Ui {
    class PETabWidget: public Ui_PETabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PETABWIDGET_H
