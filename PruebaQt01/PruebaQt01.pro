#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T14:13:07
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PruebaQt01
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp

HEADERS  += mainwindow.h \
        qcustomplot.h \
        edk.h \
        edkErrorCode.h \
        elsClient.h \
        EmoStateDLL.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -ledk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -ledk
else:unix: LIBS += -L$$PWD/lib/ -ledk

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
