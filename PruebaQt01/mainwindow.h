#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qcustomplot.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <stdio.h>

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "list"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupRealtimeDataDemo(QCustomPlot *customPlot);

    EmoEngineEventHandle eEvent;
    EmoStateHandle eState;
    unsigned int userID;
    unsigned short composerPort;
    float secs;
    unsigned int datarate;
    bool readytocollect;
    int option;
    int state;
    DataHandle hData;

    double value0_old;
    double value1_old;
    double value2_old;
    double value3_old;
    double value4_old;
    double value5_old;
    double value6_old;
    double value7_old;
    double value8_old;
    double value9_old;
    double value10_old;
    double value11_old;
    double value12_old;
    double value13_old;
    double value14_old;

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;

private slots:
    void realtimeDataSlot();
};

#endif // MAINWINDOW_H
