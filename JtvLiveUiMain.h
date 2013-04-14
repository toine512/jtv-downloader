/* This file is part of "Jtv live downloader"
 *
 * Copyright (C) 2012-2013 toine512 <toine512@gmail.com>
 *
 * "Jtv live downloader" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * "Jtv live downloader" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with "Jtv live downloader".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JTVLIVEUIMAIN_H
#define JTVLIVEUIMAIN_H

#include "defines.h"

#include <QtCore/Qt>
#include <QString>
#include <QSettings>
#include <QUuid>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QNetworkAccessManager>
#include "UpdateChecker.h"

//#include <QStatusBar>
#include <QMainWindow>
#include <QWidget>
#include <QDesktopWidget>
#include <QRect>
#include <QTabWidget>
#include <QIcon>
#include "JtvLiveUiTabJustin_tv.h"
#include "JtvLiveUiTabBasicJustin_tv.h"
#include "JtvLiveUiTabWatch.h"
#include "JtvLiveUiTabParams.h"
#include "JtvLiveUiTabRtmpdump.h"
#include "JtvLiveUiTabBasicRtmpdump.h"
#include "JtvLiveUiTabRtmpgw.h"
#include "JtvLiveUiTabAbout.h"
#include "NewUpdateTab.h"

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    ~JtvLiveUiMain();

protected slots:
    void toggleUi();
    void onGotoRecord();
    void onGotoWatchAndStart();

protected:
    //UI
    void loadAdvancedUi();
    void clearAdvancedUi();
    void loadBasicUi();
    void clearBasicUi();

    //QStatusBar *ui_bottom_statusBar;
    bool b_is_advanced_ui;
    QTabWidget *ui_widget;
    JtvLiveUiTabJustin_tv *ui_jtv;
    JtvLiveUiTabBasicJustin_tv *ui_basic_jtv;
    JtvLiveUiTabParams *ui_params;
    JtvLiveUiTabWatch *ui_watch;
    JtvLiveUiTabRtmpdump *ui_rtmpdump;
    JtvLiveUiTabBasicRtmpdump *ui_basic_rtmpdump;
    JtvLiveUiTabRtmpgw *ui_rtmpgw;
    JtvLiveUiTabAbout *ui_about;
    NewUpdateTab *ui_update;

    //Core
    QSettings *settings;
    QNetworkAccessManager *net_manager;
    JtvLiveChannel *live_channel;
    UpdateChecker *updater;

};

#endif // JTVLIVEUIMAIN_H
