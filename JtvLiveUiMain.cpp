/* This file is part of "Jtv live downloader"
 *
 * Copyright (C) 2012 toine512 <toine512@gmail.com>
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

#include "JtvLiveUiMain.h"

JtvLiveUiMain::JtvLiveUiMain(QWidget *parent) :
    QMainWindow(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
{
    /* Core */
    //QSettings setup
#ifdef Q_OS_WIN
    settings = new QSettings("jtvdl.conf", QSettings::IniFormat, this);
#else
    settings = new QSettings("$HOME/.config/jtv-downloader/jtvdl.conf", QSettings::IniFormat, this);
#endif

    //QNetworkAccessManager setup
    net_manager = new QNetworkAccessManager(this);

    //Justin.tv setup
    live_channel = new JtvLiveChannel(net_manager, settings->value("justin.tv/player", "http://fr.twitch.tv/widgets/live_embed_player.swf").toString(), settings->value("justin.tv/referer", "http://fr.twitch.tv/").toString(), this);

    //Generate an UUID for the update checker
    QString uuid = settings->value("global/uuid").toString();
    if(uuid.isEmpty())
    {
        uuid = QUuid::createUuid().toString();
        settings->setValue("global/uuid", uuid);
    }

    //UpdateChecker setup
    updater = new UpdateChecker(net_manager, "http://toine.fr.nf/jtvdl/", uuid, JTV_LIVE_VERSION, this);

    /* GUI */
    setWindowTitle("Justin.tv/Twitch.tv live downloader");

    layout()->setSizeConstraint(QLayout::SetNoConstraint); //the main layout won't resize the QMainWindow
    resize(496, 270);
    setMinimumSize(496, 270);

    //Center on the current screen
    QDesktopWidget desktop_widget;
    QRect screen_geometry = desktop_widget.availableGeometry();
    move(screen_geometry.width() / 2 - width() / 2, screen_geometry.height() / 2 - height() / 2);

    //Statusbar
    /*ui_bottom_statusBar = statusBar();
    ui_bottom_statusBar->showMessage("I'm an useless status bar !");*/

    //Central zone
    ui_widget = new QTabWidget;

    ui_jtv = new JtvLiveUiTabJustin_tv(live_channel);
    ui_watch = new JtvLiveUiTabWatch(settings, live_channel);
    ui_params = new JtvLiveUiTabParams(live_channel);
    ui_rtmpdump = new JtvLiveUiTabRtmpdump(settings, live_channel);
    ui_rtmpgw = new JtvLiveUiTabRtmpgw(settings, live_channel);
    ui_about = new JtvLiveUiTabAbout;
    ui_update = new NewUpdateTab(true, ui_widget, this);

    ui_widget->addTab(ui_jtv, "Justin.tv");
    ui_widget->addTab(ui_watch, "Watch");
    ui_widget->addTab(ui_params, "Params");
    ui_widget->addTab(ui_rtmpdump, "rtmpdump");
    ui_widget->addTab(ui_rtmpgw, "rtmpgw");
    ui_widget->addTab(ui_about, "About");

    connect(ui_jtv, SIGNAL(askClearParams()), ui_params, SLOT(clearParams()));
    connect(ui_jtv, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    connect(ui_watch, SIGNAL(askBtn_watchEnable()), ui_jtv, SLOT(btn_watchEnable()));
    connect(ui_watch, SIGNAL(askBtn_watchDisable()), ui_jtv, SLOT(btn_watchDisable()));
    connect(updater, SIGNAL(updateAvailable(const QString &, const QString &)), ui_update, SLOT(setUpdateInfos(const QString &, const QString &)));
    connect(updater, SIGNAL(updateNotes(const QString &)), ui_update, SLOT(setUpdateNotes(const QString &)));

    setCentralWidget(ui_widget);
}

void JtvLiveUiMain::onGotoWatchAndStart()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_watch));
    ui_watch->linkedProcessesStart();
}

JtvLiveUiMain::~JtvLiveUiMain()
{ }
