#include "mainwindow.h"
#include <QApplication>

#include <QtWidgets/qwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    window.show();

    return a.exec();
}
