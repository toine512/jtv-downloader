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
    qt-jtvlive/JtvLiveChannel.cpp \
    QSIRPushButton.cpp \
    UpdateChecker.cpp \
    JtvLiveUiTabJustin_tv.cpp \
    JtvLiveUiTabParams.cpp \
    JtvLiveUiTabWatch.cpp \
    JtvLiveUiTabRtmpdump.cpp \
    JtvLiveUiTabRtmpgw.cpp \
    JtvLiveUiTabAbout.cpp \
    NewUpdateTab.cpp

HEADERS  += JtvLiveUiMain.h \
    qt-jtvlive/JtvLiveChannel.h \
    defines.h \
    QSIRPushButton.h \
    UpdateChecker.h \
    JtvLiveUiTabJustin_tv.h \
    JtvLiveUiTabParams.h \
    JtvLiveUiTabWatch.h \
    JtvLiveUiTabRtmpdump.h \
    JtvLiveUiTabRtmpgw.h \
    JtvLiveUiTabAbout.h \
    NewUpdateTab.h

RESOURCES += \
    ressources.qrc
