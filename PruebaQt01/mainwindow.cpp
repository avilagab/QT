#include "mainwindow.h"
#include "ui_mainwindow.h"

EE_DataChannel_t targetChannelList[] = {
        ED_COUNTER,
        ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
        ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
        ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP,
        ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    setupRealtimeDataDemo(ui->customPlot);

    setWindowTitle("Prueba Qt - Plot Tiempo Real");
    statusBar()->clearMessage();

    eEvent			= EE_EmoEngineEventCreate();
    eState			= EE_EmoStateCreate();
    userID          = 0;
    composerPort	= 1726;
    secs			= 1;
    datarate		= 0;
    readytocollect	= false;
    option			= 0;
    state			= 0;

    value0_old = 0;
    value1_old = 0;
    value2_old = 0;
    value3_old = 0;
    value4_old = 0;


    if (EE_EngineConnect() != EDK_OK) {
        throw std::runtime_error("Emotiv Engine start up failed.");
    }

    hData = EE_DataCreate();
    EE_DataSetBufferSizeInSec(secs);

    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */

  customPlot->addGraph(); // Púrpura
  customPlot->graph(0)->setPen(QPen("#DE17DA"));
  //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  //customPlot->graph(0)->setAntialiasedFill(false);
  customPlot->addGraph(); // Amarillo
  customPlot->graph(1)->setPen(QPen("#FFCA1B"));
  //customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

  customPlot->addGraph(); // Rojo
  customPlot->graph(2)->setPen(QPen("#FF2423"));
  customPlot->addGraph(); // blue line
  customPlot->graph(3)->setPen(QPen("#13F2B3"));
  customPlot->addGraph(); // blue line
  customPlot->graph(4)->setPen(QPen("#189BFF"));/*
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));*/


  /*
  customPlot->addGraph(); // blue dot
  customPlot->graph(2)->setPen(QPen(Qt::blue));
  customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot->addGraph(); // red dot
  customPlot->graph(3)->setPen(QPen(Qt::red));
  customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
  */

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(4);
  customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.01) // at most add point every 10 ms
  {
    state = EE_EngineGetNextEvent(eEvent);

    double value0 = value0_old;
    double value1 = value1_old;
    double value2 = value2_old;
    double value3 = value3_old;
    double value4 = value4_old;

    if (state == EDK_OK)
    {
      EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
      EE_EmoEngineEventGetUserId(eEvent, &userID);

      // Log the EmoState if it has been updated
      if (eventType == EE_UserAdded)
      {
        std::cout << "User added";
        EE_DataAcquisitionEnable(userID,true);
        readytocollect = true;
      }
    }

    if (readytocollect)
    {
      EE_DataUpdateHandle(0, hData);

      unsigned int nSamplesTaken=0;
      EE_DataGetNumberOfSample(hData,&nSamplesTaken);

      std::cout << "Updated " << nSamplesTaken << std::endl;

      //Lectura de datos
      if (nSamplesTaken != 0)
      {
        double* data = new double[nSamplesTaken];
        for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx)
        {
          for (int i = 0 ; i<5 ; i++)
          {
            EE_DataGet(hData, targetChannelList[i], data, nSamplesTaken);
            switch(i)
            {
              case 0: value0 = data[sampleIdx];  value0_old = value0;  break;
              case 1: value1 = data[sampleIdx];  value1_old = value1;  break;
              case 2: value2 = data[sampleIdx];  value2_old = value2;  break;
              case 3: value3 = data[sampleIdx];  value3_old = value3;  break;
              case 4: value4 = data[sampleIdx];  value4_old = value4;  break;
            }
          }
        }
        delete[] data;
      }
    }

    //double value0 =       qSin(key*1.6 + qCos(key*1.7)*2.1)*10 + qSin(key*1.2+0.56)*20 + 26;
    //double value1 = 100 + qSin(key*1.3 + qCos(key*1.5)*1.2)*6  + qSin(key*2.0+0.26)*24 + 26;
    //double value2 = 200 + qSin(key*1.0 + qCos(key*1.7)*2.3)*15 + qSin(key*1.5+0.86)*24 + 26;
    //double value3 = 300 + qSin(key*0.7 + qCos(key*2.0)*1.2)*7  + qSin(key*1.3+0.46)*24 + 26;
    //double value4 = 400 + qSin(key*0.4 + qCos(key*1.2)*2.0)*10 + qSin(key*2.2+0.36)*24 + 26;
    // add data to lines:
    ui->customPlot->graph(0)->addData(key, value0);
    ui->customPlot->graph(1)->addData(key, value1);
    ui->customPlot->graph(2)->addData(key, value2);
    ui->customPlot->graph(3)->addData(key, value3);
    ui->customPlot->graph(4)->addData(key, value4);
    /*
    // set data of dots:
    ui->customPlot->graph(2)->clearData();
    ui->customPlot->graph(2)->addData(key, value0);
    ui->customPlot->graph(3)->clearData();
    ui->customPlot->graph(3)->addData(key, value1);
    // remove data of lines that's outside visible range:*/
    ui->customPlot->graph(0)->removeDataBefore(key-8);
    ui->customPlot->graph(1)->removeDataBefore(key-8);
    ui->customPlot->graph(2)->removeDataBefore(key-8);
    ui->customPlot->graph(3)->removeDataBefore(key-8);
    ui->customPlot->graph(4)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis(true);
    ui->customPlot->graph(1)->rescaleValueAxis(true);
    ui->customPlot->graph(2)->rescaleValueAxis(true);
    ui->customPlot->graph(3)->rescaleValueAxis(true);
    ui->customPlot->graph(4)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  ui->customPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("Estado: %1 %2")
          .arg(state)
          .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}
