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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
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
    QPushButton *Open;
    QPushButton *Close;
    QComboBox *comboBox;
    QGroupBox *Acquisition;
    QCheckBox *Continuous;
    QCheckBox *Trigger;
    QLineEdit *Time_delay;
    QLabel *label;
    QPushButton *Start;
    QPushButton *Stop;
    QPushButton *SoftwareTrigger;
    QGroupBox *Paramater;
    QLineEdit *FrameRate;
    QLabel *label_2;
    QLineEdit *Expotime;
    QLabel *label_3;
    QLabel *label_6;
    QLineEdit *Gain;
    QPushButton *SetParam;
    QPushButton *GetParam;
    QGroupBox *groupBox_3;
    QLabel *label_10;
    QLineEdit *Height;
    QLabel *label_11;
    QLineEdit *Width;
    QPushButton *Capture;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(860, 554);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Camera_View = new QLabel(centralwidget);
        Camera_View->setObjectName("Camera_View");
        Camera_View->setGeometry(QRect(20, 20, 551, 441));
        Camera_View->setFrameShape(QFrame::Box);
        Enumerate = new QPushButton(centralwidget);
        Enumerate->setObjectName("Enumerate");
        Enumerate->setGeometry(QRect(440, 470, 131, 41));
        Open = new QPushButton(centralwidget);
        Open->setObjectName("Open");
        Open->setGeometry(QRect(600, 20, 121, 41));
        Close = new QPushButton(centralwidget);
        Close->setObjectName("Close");
        Close->setGeometry(QRect(730, 20, 111, 41));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(20, 470, 401, 31));
        Acquisition = new QGroupBox(centralwidget);
        Acquisition->setObjectName("Acquisition");
        Acquisition->setGeometry(QRect(600, 70, 241, 161));
        Continuous = new QCheckBox(Acquisition);
        Continuous->setObjectName("Continuous");
        Continuous->setGeometry(QRect(120, 20, 111, 24));
        Trigger = new QCheckBox(Acquisition);
        Trigger->setObjectName("Trigger");
        Trigger->setGeometry(QRect(10, 20, 91, 24));
        Time_delay = new QLineEdit(Acquisition);
        Time_delay->setObjectName("Time_delay");
        Time_delay->setGeometry(QRect(140, 90, 91, 26));
        label = new QLabel(Acquisition);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 90, 131, 31));
        Start = new QPushButton(Acquisition);
        Start->setObjectName("Start");
        Start->setGeometry(QRect(10, 120, 111, 29));
        Stop = new QPushButton(Acquisition);
        Stop->setObjectName("Stop");
        Stop->setGeometry(QRect(120, 120, 111, 29));
        SoftwareTrigger = new QPushButton(Acquisition);
        SoftwareTrigger->setObjectName("SoftwareTrigger");
        SoftwareTrigger->setGeometry(QRect(10, 50, 211, 31));
        Paramater = new QGroupBox(centralwidget);
        Paramater->setObjectName("Paramater");
        Paramater->setGeometry(QRect(600, 240, 241, 161));
        FrameRate = new QLineEdit(Paramater);
        FrameRate->setObjectName("FrameRate");
        FrameRate->setGeometry(QRect(140, 30, 91, 26));
        label_2 = new QLabel(Paramater);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 30, 131, 31));
        Expotime = new QLineEdit(Paramater);
        Expotime->setObjectName("Expotime");
        Expotime->setGeometry(QRect(140, 60, 91, 26));
        label_3 = new QLabel(Paramater);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 60, 131, 31));
        label_6 = new QLabel(Paramater);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 90, 131, 31));
        Gain = new QLineEdit(Paramater);
        Gain->setObjectName("Gain");
        Gain->setGeometry(QRect(140, 90, 91, 26));
        SetParam = new QPushButton(Paramater);
        SetParam->setObjectName("SetParam");
        SetParam->setGeometry(QRect(130, 120, 101, 29));
        GetParam = new QPushButton(Paramater);
        GetParam->setObjectName("GetParam");
        GetParam->setGeometry(QRect(10, 120, 101, 29));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(600, 400, 241, 121));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(10, 20, 131, 31));
        Height = new QLineEdit(groupBox_3);
        Height->setObjectName("Height");
        Height->setGeometry(QRect(140, 20, 91, 26));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(10, 50, 131, 31));
        Width = new QLineEdit(groupBox_3);
        Width->setObjectName("Width");
        Width->setGeometry(QRect(140, 50, 91, 26));
        Capture = new QPushButton(groupBox_3);
        Capture->setObjectName("Capture");
        Capture->setGeometry(QRect(70, 80, 91, 31));
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
        Open->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        Close->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        Acquisition->setTitle(QCoreApplication::translate("MainWindow", "Frame Acquisition", nullptr));
        Continuous->setText(QCoreApplication::translate("MainWindow", "Continuous", nullptr));
        Trigger->setText(QCoreApplication::translate("MainWindow", "Trigger", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Trigger Delay (\302\265S)", nullptr));
        Start->setText(QCoreApplication::translate("MainWindow", "Start Grabbing", nullptr));
        Stop->setText(QCoreApplication::translate("MainWindow", "Stop Grabbing", nullptr));
        SoftwareTrigger->setText(QCoreApplication::translate("MainWindow", "Trigger", nullptr));
        Paramater->setTitle(QCoreApplication::translate("MainWindow", "Device Paramters", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Frame Rate", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Exposure Time", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Gain", nullptr));
        SetParam->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        GetParam->setText(QCoreApplication::translate("MainWindow", "Get", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Save Frame", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Height", nullptr));
        Height->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "Width", nullptr));
        Width->setText(QString());
        Capture->setText(QCoreApplication::translate("MainWindow", "Capture", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
