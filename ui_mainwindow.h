/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Mar 3 18:52:18 2012
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "gcodeview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_GCode;
    QAction *actionClose;
    QAction *actionClose_All_Except;
    QAction *actionClose_All;
    QAction *actionExport_Gcode_File;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GcodeView *graphicsView;
    QVBoxLayout *verticalLayout;
    QSlider *LayerHeight;
    QSlider *verticalSlider;
    QMenuBar *menuBar;
    QMenu *menuPathMonger;
    QMenu *menuRecent_Files;
    QMenu *menuWindow;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(953, 561);
        actionLoad_GCode = new QAction(MainWindow);
        actionLoad_GCode->setObjectName(QString::fromUtf8("actionLoad_GCode"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionClose_All_Except = new QAction(MainWindow);
        actionClose_All_Except->setObjectName(QString::fromUtf8("actionClose_All_Except"));
        actionClose_All = new QAction(MainWindow);
        actionClose_All->setObjectName(QString::fromUtf8("actionClose_All"));
        actionExport_Gcode_File = new QAction(MainWindow);
        actionExport_Gcode_File->setObjectName(QString::fromUtf8("actionExport_Gcode_File"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new GcodeView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        LayerHeight = new QSlider(centralWidget);
        LayerHeight->setObjectName(QString::fromUtf8("LayerHeight"));
        LayerHeight->setOrientation(Qt::Vertical);

        verticalLayout->addWidget(LayerHeight);


        gridLayout->addLayout(verticalLayout, 0, 2, 1, 1);

        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verticalSlider, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 953, 18));
        menuPathMonger = new QMenu(menuBar);
        menuPathMonger->setObjectName(QString::fromUtf8("menuPathMonger"));
        menuRecent_Files = new QMenu(menuPathMonger);
        menuRecent_Files->setObjectName(QString::fromUtf8("menuRecent_Files"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuPathMonger->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuPathMonger->addAction(actionLoad_GCode);
        menuPathMonger->addAction(menuRecent_Files->menuAction());
        menuPathMonger->addSeparator();
        menuPathMonger->addAction(actionClose);
        menuPathMonger->addAction(actionClose_All);
        menuPathMonger->addAction(actionClose_All_Except);
        menuPathMonger->addSeparator();
        menuPathMonger->addAction(actionExport_Gcode_File);
        menuRecent_Files->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLoad_GCode->setText(QApplication::translate("MainWindow", "Open File...", 0, QApplication::UnicodeUTF8));
        actionLoad_GCode->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionClose_All_Except->setText(QApplication::translate("MainWindow", "Close All Except", 0, QApplication::UnicodeUTF8));
        actionClose_All->setText(QApplication::translate("MainWindow", "Close All", 0, QApplication::UnicodeUTF8));
        actionClose_All->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+W", 0, QApplication::UnicodeUTF8));
        actionExport_Gcode_File->setText(QApplication::translate("MainWindow", "Export Gcode File", 0, QApplication::UnicodeUTF8));
        actionExport_Gcode_File->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        menuPathMonger->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuRecent_Files->setTitle(QApplication::translate("MainWindow", "Recent Files", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
