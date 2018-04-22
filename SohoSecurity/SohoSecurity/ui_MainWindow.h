/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "antivirustabwidget.h"
#include "cleantrashtabwidget.h"
#include "processtabwidget.h"
#include "servicetabwidget.h"
#include "windowtabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_SohoSecurityClass
{
public:
    QAction *actLogoff;
    QAction *actHibernate;
    QAction *actSleep;
    QAction *actRestart;
    QAction *actShutdown;
    QAction *actLockScreen;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    ProcessTabWidget *tabProc;
    WindowTabWidget *tabWnd;
    ServiceTabWidget *tabServ;
    CleanTrashTabWidget *tabClean;
    AntiVirusTabWidget *tabAV;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SohoSecurityClass)
    {
        if (SohoSecurityClass->objectName().isEmpty())
            SohoSecurityClass->setObjectName(QStringLiteral("SohoSecurityClass"));
        SohoSecurityClass->resize(968, 588);
        actLogoff = new QAction(SohoSecurityClass);
        actLogoff->setObjectName(QStringLiteral("actLogoff"));
        actHibernate = new QAction(SohoSecurityClass);
        actHibernate->setObjectName(QStringLiteral("actHibernate"));
        actSleep = new QAction(SohoSecurityClass);
        actSleep->setObjectName(QStringLiteral("actSleep"));
        actRestart = new QAction(SohoSecurityClass);
        actRestart->setObjectName(QStringLiteral("actRestart"));
        actShutdown = new QAction(SohoSecurityClass);
        actShutdown->setObjectName(QStringLiteral("actShutdown"));
        actLockScreen = new QAction(SohoSecurityClass);
        actLockScreen->setObjectName(QStringLiteral("actLockScreen"));
        centralWidget = new QWidget(SohoSecurityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 951, 541));
        tabProc = new ProcessTabWidget();
        tabProc->setObjectName(QStringLiteral("tabProc"));
        tabWidget->addTab(tabProc, QString());
        tabWnd = new WindowTabWidget();
        tabWnd->setObjectName(QStringLiteral("tabWnd"));
        tabWidget->addTab(tabWnd, QString());
        tabServ = new ServiceTabWidget();
        tabServ->setObjectName(QStringLiteral("tabServ"));
        tabWidget->addTab(tabServ, QString());
        tabClean = new CleanTrashTabWidget();
        tabClean->setObjectName(QStringLiteral("tabClean"));
        tabWidget->addTab(tabClean, QString());
        tabAV = new AntiVirusTabWidget();
        tabAV->setObjectName(QStringLiteral("tabAV"));
        tabWidget->addTab(tabAV, QString());
        SohoSecurityClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SohoSecurityClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 968, 21));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu->setToolTipsVisible(true);
        SohoSecurityClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SohoSecurityClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SohoSecurityClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SohoSecurityClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SohoSecurityClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actLockScreen);
        menu->addAction(actLogoff);
        menu->addAction(actHibernate);
        menu->addAction(actSleep);
        menu->addAction(actRestart);
        menu->addAction(actShutdown);

        retranslateUi(SohoSecurityClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SohoSecurityClass);
    } // setupUi

    void retranslateUi(QMainWindow *SohoSecurityClass)
    {
        SohoSecurityClass->setWindowTitle(QApplication::translate("SohoSecurityClass", "SohoSecurity", 0));
        actLogoff->setText(QApplication::translate("SohoSecurityClass", "\346\263\250\351\224\200(&O)", 0));
        actLogoff->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+O", 0));
        actHibernate->setText(QApplication::translate("SohoSecurityClass", "\344\274\221\347\234\240(&H)", 0));
        actHibernate->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+H", 0));
        actSleep->setText(QApplication::translate("SohoSecurityClass", "\347\235\241\347\234\240(&S)", 0));
        actSleep->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+S", 0));
        actRestart->setText(QApplication::translate("SohoSecurityClass", "\351\207\215\345\220\257(&R)", 0));
        actRestart->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+R", 0));
        actShutdown->setText(QApplication::translate("SohoSecurityClass", "\345\205\263\346\234\272(&P)", 0));
        actShutdown->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+P", 0));
        actLockScreen->setText(QApplication::translate("SohoSecurityClass", "\351\224\201\345\261\217(&L)", 0));
        actLockScreen->setShortcut(QApplication::translate("SohoSecurityClass", "Alt+L", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabProc), QApplication::translate("SohoSecurityClass", "\350\277\233\347\250\213", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabWnd), QApplication::translate("SohoSecurityClass", "\347\252\227\345\217\243", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabServ), QApplication::translate("SohoSecurityClass", "\346\234\215\345\212\241", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabClean), QApplication::translate("SohoSecurityClass", "\345\236\203\345\234\276\346\270\205\347\220\206", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabAV), QApplication::translate("SohoSecurityClass", "\347\227\205\346\257\222\346\237\245\346\235\200", 0));
        menu->setTitle(QApplication::translate("SohoSecurityClass", "\346\216\247\345\210\266(&M)", 0));
    } // retranslateUi

};

namespace Ui {
    class SohoSecurityClass: public Ui_SohoSecurityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
