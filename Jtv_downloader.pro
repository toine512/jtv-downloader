#-------------------------------------------------
#
# Project created by QtCreator 2011-07-22T11:25:03
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = jtv-downloader
TEMPLATE = app


SOURCES += main.cpp\
        JtvLiveUiMain.cpp \
    JtvLiveChannel.cpp \
    QSIRPushButton.cpp

HEADERS  += JtvLiveUiMain.h \
    JtvLiveChannel.h \
    defines.h \
    QSIRPushButton.h

RESOURCES += \
    ressources.qrc
