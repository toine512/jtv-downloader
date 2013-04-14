#-------------------------------------------------
#
# Project created by QtCreator 2011-07-22T11:25:03
#
#-------------------------------------------------

QT += core gui network xml

TARGET = jtv-downloader
TEMPLATE = app


SOURCES += main.cpp\
           JtvLiveUiMain.cpp \
           qt-jtvlive/JtvLiveChannel.cpp \
           UpdateChecker.cpp \
           JtvLiveUiTabJustin_tv.cpp \
           JtvLiveUiTabParams.cpp \
           JtvLiveUiTabWatch.cpp \
           JtvLiveUiTabRtmpdump.cpp \
           JtvLiveUiTabRtmpgw.cpp \
           JtvLiveUiTabAbout.cpp \
           qt-shared/buttons/QSquarePushButton.cpp \
           qt-shared/buttons/QSquareIconResizingPushButton.cpp \
           NewUpdateTab.cpp \
           JtvLiveUiTabBasicJustin_tv.cpp \
           JtvLiveUiTabBasicRtmpdump.cpp

HEADERS += defines.h \
           JtvLiveUiMain.h \
           qt-jtvlive/JtvLiveChannel.h \
           UpdateChecker.h \
           JtvLiveUiTabJustin_tv.h \
           JtvLiveUiTabParams.h \
           JtvLiveUiTabWatch.h \
           JtvLiveUiTabRtmpdump.h \
           JtvLiveUiTabRtmpgw.h \
           JtvLiveUiTabAbout.h \
           qt-shared/buttons/QSquarePushButton.h \
           qt-shared/buttons/QSquareIconResizingPushButton.h \
           NewUpdateTab.h \
           JtvLiveUiTabBasicJustin_tv.h \
           JtvLiveUiTabBasicRtmpdump.h

RESOURCES += ressources.qrc

RC_FILE += jtv-downloader.rc
