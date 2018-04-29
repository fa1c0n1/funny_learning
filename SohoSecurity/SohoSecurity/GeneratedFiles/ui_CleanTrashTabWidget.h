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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CleanTrashTabWidget
{
public:
    QGroupBox *groupBox;
    QPushButton *psBtnClnSysAndBrws;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelTmpFiles;
    QLabel *labelSystemFiles;
    QLabel *labelRecyBinFiles;
    QLabel *labelSysCacheFiles;
    QLabel *labelLogFiles;
    QLabel *labelBrowserCache;
    QGroupBox *groupBox_2;
    QListWidget *listWidgetVSPath;
    QLabel *label;
    QTextBrowser *textBrwVSDelFile;
    QPushButton *psBtnCleanVSprj;

    void setupUi(QWidget *CleanTrashTabWidget)
    {
        if (CleanTrashTabWidget->objectName().isEmpty())
            CleanTrashTabWidget->setObjectName(QStringLiteral("CleanTrashTabWidget"));
        CleanTrashTabWidget->resize(940, 519);
        groupBox = new QGroupBox(CleanTrashTabWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 921, 161));
        psBtnClnSysAndBrws = new QPushButton(groupBox);
        psBtnClnSysAndBrws->setObjectName(QStringLiteral("psBtnClnSysAndBrws"));
        psBtnClnSysAndBrws->setGeometry(QRect(840, 130, 75, 23));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(22, 22, 74, 131));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_5);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_6);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_7);

        widget1 = new QWidget(groupBox);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(100, 20, 731, 131));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelTmpFiles = new QLabel(widget1);
        labelTmpFiles->setObjectName(QStringLiteral("labelTmpFiles"));

        verticalLayout_2->addWidget(labelTmpFiles);

        labelSystemFiles = new QLabel(widget1);
        labelSystemFiles->setObjectName(QStringLiteral("labelSystemFiles"));

        verticalLayout_2->addWidget(labelSystemFiles);

        labelRecyBinFiles = new QLabel(widget1);
        labelRecyBinFiles->setObjectName(QStringLiteral("labelRecyBinFiles"));

        verticalLayout_2->addWidget(labelRecyBinFiles);

        labelSysCacheFiles = new QLabel(widget1);
        labelSysCacheFiles->setObjectName(QStringLiteral("labelSysCacheFiles"));

        verticalLayout_2->addWidget(labelSysCacheFiles);

        labelLogFiles = new QLabel(widget1);
        labelLogFiles->setObjectName(QStringLiteral("labelLogFiles"));

        verticalLayout_2->addWidget(labelLogFiles);

        labelBrowserCache = new QLabel(widget1);
        labelBrowserCache->setObjectName(QStringLiteral("labelBrowserCache"));

        verticalLayout_2->addWidget(labelBrowserCache);

        groupBox_2 = new QGroupBox(CleanTrashTabWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 180, 921, 331));
        listWidgetVSPath = new QListWidget(groupBox_2);
        listWidgetVSPath->setObjectName(QStringLiteral("listWidgetVSPath"));
        listWidgetVSPath->setGeometry(QRect(10, 40, 351, 281));
        listWidgetVSPath->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidgetVSPath->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidgetVSPath->setSortingEnabled(true);
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 371, 16));
        textBrwVSDelFile = new QTextBrowser(groupBox_2);
        textBrwVSDelFile->setObjectName(QStringLiteral("textBrwVSDelFile"));
        textBrwVSDelFile->setGeometry(QRect(370, 20, 461, 301));
        psBtnCleanVSprj = new QPushButton(groupBox_2);
        psBtnCleanVSprj->setObjectName(QStringLiteral("psBtnCleanVSprj"));
        psBtnCleanVSprj->setGeometry(QRect(840, 300, 75, 23));

        retranslateUi(CleanTrashTabWidget);
        QObject::connect(psBtnClnSysAndBrws, SIGNAL(clicked()), CleanTrashTabWidget, SLOT(onPsBtnClnSysAndBrwsClicked()));
        QObject::connect(psBtnCleanVSprj, SIGNAL(clicked()), CleanTrashTabWidget, SLOT(onpsBtnCleanVSprjClicked()));

        QMetaObject::connectSlotsByName(CleanTrashTabWidget);
    } // setupUi

    void retranslateUi(QWidget *CleanTrashTabWidget)
    {
        CleanTrashTabWidget->setWindowTitle(QApplication::translate("CleanTrashTabWidget", "CleanTrashTabWidget", 0));
        groupBox->setTitle(QApplication::translate("CleanTrashTabWidget", "\347\263\273\347\273\237\345\222\214\346\265\217\350\247\210\345\231\250\345\236\203\345\234\276\346\270\205\347\220\206", 0));
        psBtnClnSysAndBrws->setText(QApplication::translate("CleanTrashTabWidget", "\344\270\200\351\224\256\346\270\205\347\220\206", 0));
        label_2->setText(QApplication::translate("CleanTrashTabWidget", "\344\270\264\346\227\266\346\226\207\344\273\266\357\274\232", 0));
        label_3->setText(QApplication::translate("CleanTrashTabWidget", "\347\263\273\347\273\237\346\226\207\344\273\266\357\274\232", 0));
        label_4->setText(QApplication::translate("CleanTrashTabWidget", " \345\233\236\346\224\266\347\253\231\357\274\232", 0));
        label_5->setText(QApplication::translate("CleanTrashTabWidget", "\347\263\273\347\273\237\347\274\223\345\255\230\357\274\232", 0));
        label_6->setText(QApplication::translate("CleanTrashTabWidget", "\346\227\245\345\277\227\346\226\207\344\273\266\357\274\232", 0));
        label_7->setText(QApplication::translate("CleanTrashTabWidget", "\346\265\217\350\247\210\345\231\250\345\236\203\345\234\276\357\274\232", 0));
        labelTmpFiles->setText(QString());
        labelSystemFiles->setText(QString());
        labelRecyBinFiles->setText(QString());
        labelSysCacheFiles->setText(QString());
        labelLogFiles->setText(QString());
        labelBrowserCache->setText(QString());
        groupBox_2->setTitle(QApplication::translate("CleanTrashTabWidget", "VS\345\267\245\347\250\213\346\270\205\347\220\206", 0));
        label->setText(QApplication::translate("CleanTrashTabWidget", "\350\257\267\345\260\206\350\246\201\346\270\205\347\220\206\347\232\204VS\345\267\245\347\250\213\347\233\256\345\275\225\346\213\226\346\213\275\345\210\260\344\270\213\351\235\242\347\232\204\345\210\227\350\241\250\346\241\206\344\270\255. Del\351\224\256\345\217\257\351\200\211\344\270\255\345\210\240\351\231\244", 0));
        psBtnCleanVSprj->setText(QApplication::translate("CleanTrashTabWidget", "\345\274\200\345\247\213\346\270\205\347\220\206", 0));
    } // retranslateUi

};

namespace Ui {
    class CleanTrashTabWidget: public Ui_CleanTrashTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLEANTRASHTABWIDGET_H
