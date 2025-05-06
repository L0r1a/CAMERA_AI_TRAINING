/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *Camera_View;
    QPushButton *Enumerate;
    QPushButton *Start;
    QPushButton *Stop;
    QComboBox *comboBox;
    QLabel *label;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(531, 385);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Camera_View = new QLabel(centralwidget);
        Camera_View->setObjectName("Camera_View");
        Camera_View->setGeometry(QRect(20, 60, 491, 241));
        Camera_View->setFrameShape(QFrame::Box);
        Enumerate = new QPushButton(centralwidget);
        Enumerate->setObjectName("Enumerate");
        Enumerate->setGeometry(QRect(20, 310, 101, 41));
        Start = new QPushButton(centralwidget);
        Start->setObjectName("Start");
        Start->setGeometry(QRect(290, 310, 101, 41));
        Stop = new QPushButton(centralwidget);
        Stop->setObjectName("Stop");
        Stop->setGeometry(QRect(410, 310, 101, 41));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(20, 20, 201, 26));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(230, 20, 271, 21));
        label->setFrameShape(QFrame::Box);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Camera_View->setText(QString());
        Enumerate->setText(QCoreApplication::translate("MainWindow", "Find Device", nullptr));
        Start->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        Stop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
