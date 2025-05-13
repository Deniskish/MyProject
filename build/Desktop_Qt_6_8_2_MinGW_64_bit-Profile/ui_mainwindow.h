/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <openglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit_FILENAME;
    QLineEdit *lineEditCoordinate;
    OpenGLWidget *glWidget;
    QLineEdit *lineEdit_NewContur;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(680, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");

        gridLayout->addWidget(pushButton_3, 3, 1, 1, 1);

        lineEdit_FILENAME = new QLineEdit(centralwidget);
        lineEdit_FILENAME->setObjectName("lineEdit_FILENAME");
        lineEdit_FILENAME->setMouseTracking(false);
        lineEdit_FILENAME->setTabletTracking(false);
        lineEdit_FILENAME->setAcceptDrops(false);
        lineEdit_FILENAME->setReadOnly(true);

        gridLayout->addWidget(lineEdit_FILENAME, 0, 0, 1, 1);

        lineEditCoordinate = new QLineEdit(centralwidget);
        lineEditCoordinate->setObjectName("lineEditCoordinate");

        gridLayout->addWidget(lineEditCoordinate, 2, 0, 1, 1);

        glWidget = new OpenGLWidget(centralwidget);
        glWidget->setObjectName("glWidget");

        gridLayout->addWidget(glWidget, 1, 0, 1, 2);

        lineEdit_NewContur = new QLineEdit(centralwidget);
        lineEdit_NewContur->setObjectName("lineEdit_NewContur");

        gridLayout->addWidget(lineEdit_NewContur, 3, 0, 1, 1);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setMouseTracking(false);
        pushButton->setTabletTracking(false);
        pushButton->setAcceptDrops(false);
        pushButton->setAutoDefault(false);

        gridLayout->addWidget(pushButton, 0, 1, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout->addWidget(pushButton_2, 2, 1, 1, 1);

        gridLayout->setRowStretch(0, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 680, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        pushButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "newContur", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PUSH inFile", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "coordinatePoint", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
