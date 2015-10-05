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
    value5_old = 0;
    value6_old = 0;
    value7_old = 0;
    value8_old = 0;
    value9_old = 0;
    value10_old = 0;
    value11_old = 0;
    value12_old = 0;
    value13_old = 0;
    value14_old = 0;


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

  customPlot->addGraph(); // Púrpura
  customPlot->graph(0)->setPen(QPen("#DE17DA"));
  customPlot->addGraph(); // Amarillo
  customPlot->graph(1)->setPen(QPen("#FFCA1B"));
  customPlot->addGraph(); // Rojo
  customPlot->graph(2)->setPen(QPen("#FF2423"));
  customPlot->addGraph(); // Color?
  customPlot->graph(3)->setPen(QPen("#13F2B3"));
  customPlot->addGraph(); // Color?
  customPlot->graph(4)->setPen(QPen("#189BFF"));
  customPlot->addGraph(); // Color?
  customPlot->graph(5)->setPen(QPen("#18FF89"));
  customPlot->addGraph(); // Color?
  customPlot->graph(6)->setPen(QPen("#FF9B89"));
  customPlot->addGraph(); // Color?
  customPlot->graph(7)->setPen(QPen("#13FF23"));
  customPlot->addGraph(); // Color?
  customPlot->graph(8)->setPen(QPen("#1324FF"));
  customPlot->addGraph(); // Color?
  customPlot->graph(9)->setPen(QPen("#18FFB3"));
  customPlot->addGraph(); // Color?
  customPlot->graph(10)->setPen(QPen("#189BFF"));
  customPlot->addGraph(); // Color?
  customPlot->graph(11)->setPen(QPen("#FF2389"));
  customPlot->addGraph(); // Color?
  customPlot->graph(12)->setPen(QPen("#23FF89"));
  customPlot->addGraph(); // Color?
  customPlot->graph(13)->setPen(QPen("#899BFF"));
  customPlot->addGraph(); // Color?
  customPlot->graph(14)->setPen(QPen("#189BFF"));

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
    double value5 = value5_old;
    double value6 = value6_old;
    double value7 = value7_old;
    double value8 = value8_old;
    double value9 = value9_old;
    double value10 = value10_old;
    double value11 = value11_old;
    double value12 = value12_old;
    double value13 = value13_old;
    double value14 = value14_old;

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
          for (int i = 0 ; i<15 ; i++)
          {
            EE_DataGet(hData, targetChannelList[i], data, nSamplesTaken);
            switch(i)
            {
              case 0:  if(ui->ED_COUNTER->isChecked()) {
                         value0 = data[sampleIdx];
                         value0_old = value0;
                         ui->customPlot->graph(0)->addData(key, value0); }
                       break;
              case 1:  if(ui->ED_AF3->isChecked()) {
                         value1 = data[sampleIdx];
                         value1_old = value1;
                         ui->customPlot->graph(1)->addData(key, value1); }
                       break;
              case 2:  if(ui->ED_F7->isChecked()) {
                         value2 = data[sampleIdx];
                         value2_old = value2;
                         ui->customPlot->graph(2)->addData(key, value2); }
                       break;
              case 3:  if(ui->ED_F3->isChecked()) {
                         value3 = data[sampleIdx];
                         value3_old = value3;
                         ui->customPlot->graph(3)->addData(key, value3); }
                       break;
              case 4:  if(ui->ED_FC5->isChecked()) {
                         value4 = data[sampleIdx];
                         value4_old = value4;
                         ui->customPlot->graph(4)->addData(key, value4); }
                       break;
              case 5:  if(ui->ED_T7->isChecked()) {
                         value5 = data[sampleIdx];
                         value5_old = value5;
                         ui->customPlot->graph(5)->addData(key, value5); }
                       break;
              case 6:  if(ui->ED_P7->isChecked()) {
                         value6 = data[sampleIdx];
                         value6_old = value6;
                         ui->customPlot->graph(6)->addData(key, value6); }
                       break;
              case 7:  if(ui->ED_O1->isChecked()) {
                         value7 = data[sampleIdx];
                         value7_old = value7;
                         ui->customPlot->graph(7)->addData(key, value7); }
                       break;
              case 8:  if(ui->ED_O2->isChecked()) {
                         value8 = data[sampleIdx];
                         value8_old = value8;
                         ui->customPlot->graph(8)->addData(key, value8); }
                       break;
              case 9:  if(ui->ED_P8->isChecked()) {
                         value9 = data[sampleIdx];
                         value9_old = value9;
                         ui->customPlot->graph(9)->addData(key, value9); }
                       break;
              case 10: if(ui->ED_T8->isChecked()) {
                         value10 = data[sampleIdx];
                         value10_old = value10;
                         ui->customPlot->graph(10)->addData(key, value10); }
                       break;
              case 11: if(ui->ED_FC6->isChecked()) {
                         value11 = data[sampleIdx];
                         value11_old = value11;
                         ui->customPlot->graph(11)->addData(key, value11); }
                       break;
              case 12: if(ui->ED_F4->isChecked()) {
                         value12 = data[sampleIdx];
                         value12_old = value12;
                         ui->customPlot->graph(12)->addData(key, value12); }
                       break;
              case 13: if(ui->ED_F8->isChecked()) {
                         value13 = data[sampleIdx];
                         value13_old = value13;
                         ui->customPlot->graph(13)->addData(key, value13); }
                       break;
              case 14: if(ui->ED_AF4->isChecked()) {
                         value14 = data[sampleIdx];
                         value14_old = value14;
                         ui->customPlot->graph(14)->addData(key, value14); }
                       break;
            }
          }
        }
        delete[] data;
      }
    }

    // add data to lines:

    ui->customPlot->graph(0)->removeDataBefore(key-8);
    ui->customPlot->graph(1)->removeDataBefore(key-8);
    ui->customPlot->graph(2)->removeDataBefore(key-8);
    ui->customPlot->graph(3)->removeDataBefore(key-8);
    ui->customPlot->graph(4)->removeDataBefore(key-8);
    ui->customPlot->graph(5)->removeDataBefore(key-8);
    ui->customPlot->graph(6)->removeDataBefore(key-8);
    ui->customPlot->graph(7)->removeDataBefore(key-8);
    ui->customPlot->graph(8)->removeDataBefore(key-8);
    ui->customPlot->graph(9)->removeDataBefore(key-8);
    ui->customPlot->graph(10)->removeDataBefore(key-8);
    ui->customPlot->graph(11)->removeDataBefore(key-8);
    ui->customPlot->graph(12)->removeDataBefore(key-8);
    ui->customPlot->graph(13)->removeDataBefore(key-8);
    ui->customPlot->graph(14)->removeDataBefore(key-8);

    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis(true);
    ui->customPlot->graph(1)->rescaleValueAxis(true);
    ui->customPlot->graph(2)->rescaleValueAxis(true);
    ui->customPlot->graph(3)->rescaleValueAxis(true);
    ui->customPlot->graph(4)->rescaleValueAxis(true);
    ui->customPlot->graph(5)->rescaleValueAxis(true);
    ui->customPlot->graph(6)->rescaleValueAxis(true);
    ui->customPlot->graph(7)->rescaleValueAxis(true);
    ui->customPlot->graph(8)->rescaleValueAxis(true);
    ui->customPlot->graph(9)->rescaleValueAxis(true);
    ui->customPlot->graph(10)->rescaleValueAxis(true);
    ui->customPlot->graph(11)->rescaleValueAxis(true);
    ui->customPlot->graph(12)->rescaleValueAxis(true);
    ui->customPlot->graph(13)->rescaleValueAxis(true);
    ui->customPlot->graph(14)->rescaleValueAxis(true);
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
