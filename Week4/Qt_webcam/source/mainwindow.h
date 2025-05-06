#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QCloseEvent>
#include <QMessageBox>
#include <thread>
#include <QButtonGroup>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    static void captureRun(QLabel *label1, QLabel *label2, QCheckBox *Reset,  QCheckBox*Grayscale,  QCheckBox *Threshold, QCheckBox *Mean);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
