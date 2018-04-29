/********************************************************************************
** Form generated from reading UI file 'OffsetCounterDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OFFSETCOUNTERDLG_H
#define UI_OFFSETCOUNTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OffsetCounterDlg
{
public:
    QGroupBox *groupBox;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *psBtnRVA;
    QLineEdit *leRVA;
    QPushButton *psBtnFOA;
    QLineEdit *leFOA;
    QPushButton *psBtnExecCount;
    QGroupBox *groupBox_2;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leSectName;

    void setupUi(QDialog *OffsetCounterDlg)
    {
        if (OffsetCounterDlg->objectName().isEmpty())
            OffsetCounterDlg->setObjectName(QStringLiteral("OffsetCounterDlg"));
        OffsetCounterDlg->resize(402, 164);
        groupBox = new QGroupBox(OffsetCounterDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 321, 81));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(40, 21, 251, 54));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        psBtnRVA = new QPushButton(widget);
        psBtnRVA->setObjectName(QStringLiteral("psBtnRVA"));

        gridLayout->addWidget(psBtnRVA, 0, 0, 1, 1);

        leRVA = new QLineEdit(widget);
        leRVA->setObjectName(QStringLiteral("leRVA"));
        leRVA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(leRVA, 0, 1, 1, 1);

        psBtnFOA = new QPushButton(widget);
        psBtnFOA->setObjectName(QStringLiteral("psBtnFOA"));

        gridLayout->addWidget(psBtnFOA, 1, 0, 1, 1);

        leFOA = new QLineEdit(widget);
        leFOA->setObjectName(QStringLiteral("leFOA"));
        leFOA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(leFOA, 1, 1, 1, 1);

        psBtnRVA->raise();
        psBtnFOA->raise();
        leRVA->raise();
        leFOA->raise();
        psBtnExecCount = new QPushButton(OffsetCounterDlg);
        psBtnExecCount->setObjectName(QStringLiteral("psBtnExecCount"));
        psBtnExecCount->setGeometry(QRect(340, 40, 51, 31));
        groupBox_2 = new QGroupBox(OffsetCounterDlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 100, 321, 61));
        widget1 = new QWidget(groupBox_2);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(70, 20, 201, 22));
        horizontalLayout = new QHBoxLayout(widget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        leSectName = new QLineEdit(widget1);
        leSectName->setObjectName(QStringLiteral("leSectName"));
        leSectName->setAlignment(Qt::AlignCenter);
        leSectName->setReadOnly(true);

        horizontalLayout->addWidget(leSectName);


        retranslateUi(OffsetCounterDlg);
        QObject::connect(psBtnRVA, SIGNAL(clicked()), OffsetCounterDlg, SLOT(onPsBtnRVAClicked()));
        QObject::connect(psBtnFOA, SIGNAL(clicked()), OffsetCounterDlg, SLOT(onPsBtnFOAClicked()));
        QObject::connect(psBtnExecCount, SIGNAL(clicked()), OffsetCounterDlg, SLOT(onPsBtnExecCountClicked()));

        QMetaObject::connectSlotsByName(OffsetCounterDlg);
    } // setupUi

    void retranslateUi(QDialog *OffsetCounterDlg)
    {
        OffsetCounterDlg->setWindowTitle(QApplication::translate("OffsetCounterDlg", "\344\275\215\347\275\256\350\256\241\347\256\227\345\231\250", 0));
        groupBox->setTitle(QApplication::translate("OffsetCounterDlg", "\345\234\260\345\235\200", 0));
        psBtnRVA->setText(QApplication::translate("OffsetCounterDlg", "\347\233\270\345\257\271\350\231\232\346\213\237\345\234\260\345\235\200(RVA)", 0));
        psBtnFOA->setText(QApplication::translate("OffsetCounterDlg", "\346\226\207\344\273\266\345\201\217\347\247\273\345\234\260\345\235\200(FOA)", 0));
        psBtnExecCount->setText(QApplication::translate("OffsetCounterDlg", "\350\256\241\347\256\227", 0));
        groupBox_2->setTitle(QApplication::translate("OffsetCounterDlg", "\344\277\241\346\201\257", 0));
        label->setText(QApplication::translate("OffsetCounterDlg", "\346\211\200\345\234\250\345\214\272\346\256\265\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class OffsetCounterDlg: public Ui_OffsetCounterDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OFFSETCOUNTERDLG_H
