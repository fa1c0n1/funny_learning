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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AntiVirusTabWidget
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QListWidget *listWgVirus;
    QPushButton *psBtnLocalAV;
    QPushButton *psBtnCloudAV;
    QLabel *label_2;
    QLineEdit *leScanPath;
    QPushButton *psBtnSelectDir;
    QLabel *labelFilePath;
    QLabel *labelScanResult;
    QGroupBox *groupBox_2;
    QTableWidget *tableWgSuspProc;
    QLabel *label_3;
    QPushButton *psBtnProcScan;
    QLabel *labelProcess;

    void setupUi(QWidget *AntiVirusTabWidget)
    {
        if (AntiVirusTabWidget->objectName().isEmpty())
            AntiVirusTabWidget->setObjectName(QStringLiteral("AntiVirusTabWidget"));
        AntiVirusTabWidget->resize(952, 487);
        groupBox = new QGroupBox(AntiVirusTabWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 931, 291));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 81, 21));
        listWgVirus = new QListWidget(groupBox);
        listWgVirus->setObjectName(QStringLiteral("listWgVirus"));
        listWgVirus->setGeometry(QRect(10, 130, 821, 151));
        listWgVirus->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWgVirus->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWgVirus->setSortingEnabled(true);
        psBtnLocalAV = new QPushButton(groupBox);
        psBtnLocalAV->setObjectName(QStringLiteral("psBtnLocalAV"));
        psBtnLocalAV->setGeometry(QRect(840, 220, 75, 23));
        psBtnCloudAV = new QPushButton(groupBox);
        psBtnCloudAV->setObjectName(QStringLiteral("psBtnCloudAV"));
        psBtnCloudAV->setGeometry(QRect(840, 250, 75, 23));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 101, 16));
        leScanPath = new QLineEdit(groupBox);
        leScanPath->setObjectName(QStringLiteral("leScanPath"));
        leScanPath->setGeometry(QRect(110, 30, 721, 20));
        leScanPath->setReadOnly(true);
        psBtnSelectDir = new QPushButton(groupBox);
        psBtnSelectDir->setObjectName(QStringLiteral("psBtnSelectDir"));
        psBtnSelectDir->setGeometry(QRect(840, 30, 75, 23));
        labelFilePath = new QLabel(groupBox);
        labelFilePath->setObjectName(QStringLiteral("labelFilePath"));
        labelFilePath->setGeometry(QRect(110, 60, 721, 20));
        labelScanResult = new QLabel(groupBox);
        labelScanResult->setObjectName(QStringLiteral("labelScanResult"));
        labelScanResult->setGeometry(QRect(10, 110, 381, 16));
        groupBox_2 = new QGroupBox(AntiVirusTabWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 310, 931, 171));
        tableWgSuspProc = new QTableWidget(groupBox_2);
        tableWgSuspProc->setObjectName(QStringLiteral("tableWgSuspProc"));
        tableWgSuspProc->setGeometry(QRect(10, 40, 821, 121));
        tableWgSuspProc->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWgSuspProc->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWgSuspProc->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWgSuspProc->setSortingEnabled(true);
        tableWgSuspProc->verticalHeader()->setVisible(true);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 20, 81, 16));
        psBtnProcScan = new QPushButton(groupBox_2);
        psBtnProcScan->setObjectName(QStringLiteral("psBtnProcScan"));
        psBtnProcScan->setGeometry(QRect(840, 130, 75, 23));
        labelProcess = new QLabel(groupBox_2);
        labelProcess->setObjectName(QStringLiteral("labelProcess"));
        labelProcess->setGeometry(QRect(70, 20, 761, 16));

        retranslateUi(AntiVirusTabWidget);
        QObject::connect(psBtnSelectDir, SIGNAL(clicked()), AntiVirusTabWidget, SLOT(onSelectScanPath()));
        QObject::connect(psBtnLocalAV, SIGNAL(clicked()), AntiVirusTabWidget, SLOT(onPsBtnLocalAVClicked()));
        QObject::connect(psBtnCloudAV, SIGNAL(clicked()), AntiVirusTabWidget, SLOT(onPsBtnCloudAVClicked()));
        QObject::connect(psBtnProcScan, SIGNAL(clicked()), AntiVirusTabWidget, SLOT(onPsBtnProcScanClicked()));

        QMetaObject::connectSlotsByName(AntiVirusTabWidget);
    } // setupUi

    void retranslateUi(QWidget *AntiVirusTabWidget)
    {
        AntiVirusTabWidget->setWindowTitle(QApplication::translate("AntiVirusTabWidget", "AntiVirusTabWidget", 0));
        groupBox->setTitle(QApplication::translate("AntiVirusTabWidget", "\346\234\254\345\234\260\346\226\207\344\273\266MD5\346\237\245\346\235\200/\344\272\221\346\237\245\346\235\200", 0));
        label->setText(QApplication::translate("AntiVirusTabWidget", "\346\211\253\346\217\217\346\226\207\344\273\266\345\220\215\357\274\232", 0));
        psBtnLocalAV->setText(QApplication::translate("AntiVirusTabWidget", "\346\234\254\345\234\260\346\237\245\346\235\200", 0));
        psBtnCloudAV->setText(QApplication::translate("AntiVirusTabWidget", "\344\272\221\346\237\245\346\235\200", 0));
        label_2->setText(QApplication::translate("AntiVirusTabWidget", "\350\257\267\346\214\207\345\256\232\346\211\253\346\217\217\350\267\257\345\276\204\357\274\232", 0));
        psBtnSelectDir->setText(QApplication::translate("AntiVirusTabWidget", "\351\200\211\346\213\251\347\233\256\345\275\225", 0));
        labelFilePath->setText(QString());
        labelScanResult->setText(QString());
        groupBox_2->setTitle(QApplication::translate("AntiVirusTabWidget", "\350\277\233\347\250\213\346\237\245\346\235\200", 0));
        label_3->setText(QApplication::translate("AntiVirusTabWidget", "\346\211\253\346\217\217\350\277\233\347\250\213\357\274\232", 0));
        psBtnProcScan->setText(QApplication::translate("AntiVirusTabWidget", "\350\277\233\347\250\213\346\237\245\346\235\200", 0));
        labelProcess->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AntiVirusTabWidget: public Ui_AntiVirusTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANTIVIRUSTABWIDGET_H
