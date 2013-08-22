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

#include "JtvLiveUiMain.h"

JtvLiveUiMain::JtvLiveUiMain(QWidget *parent) :
    QMainWindow(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
{
    /* Core */
    //QSettings setup
#ifdef Q_OS_WIN
    settings = new QSettings("jtvdl.ini", QSettings::IniFormat, this);
#else
    //settings = new QSettings("$HOME/.config/jtv-downloader/jtvdl.conf", QSettings::IniFormat, this);
    //Config file should be located in $HOME/.config/toine512/jtvdl.ini
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "toine512", "jtvdl", this);
#endif

    //QNetworkAccessManager setup
    net_manager = new QNetworkAccessManager(this);

    //Justin.tv back-end setup
    live_channel = new JtvLiveChannel(net_manager, settings->value("justin.tv/player", "http://fr.twitch.tv/widgets/live_embed_player.swf").toString(), settings->value("justin.tv/referer", "http://fr.twitch.tv/").toString(), this);

    //Generate an UUID for the update checker
    QString uuid = settings->value("global/uuid").toString();
    if(uuid.isEmpty())
    {
        uuid = QUuid::createUuid().toString();
        settings->setValue("global/uuid", uuid);
    }

    //UpdateChecker setup
    updater = new UpdateChecker(net_manager, "http://upd.toine512.fr/jtvdl/", uuid, JTV_LIVE_VERSION, this);

    /* GUI */
    setWindowTitle("Justin.tv / Twitch.tv live downloader");

    //Central main widget
    ui_widget = new QTabWidget;

    //Create the tab for the updater
    ui_update = new NewUpdateTab(true, ui_widget, this);
    connect(updater, SIGNAL(updateAvailable(const QString &, const QString &)), ui_update, SLOT(setUpdateInfos(const QString &, const QString &)));
    connect(updater, SIGNAL(updateNotes(const QString &)), ui_update, SLOT(setUpdateNotes(const QString &)));

    //Which interface do we load ?
    b_is_advanced_ui = settings->value("gui/advanced", false).toBool();
    if(b_is_advanced_ui) //Load advanced interface
    {
        loadAdvancedUi();
    }
    else //Load simplified interface
    {
        loadBasicUi();
    }

    //Statusbar
    /*ui_bottom_statusBar = statusBar();
    ui_bottom_statusBar->showMessage("I'm an useless status bar !");*/

    //Center the window on the current screen
    QDesktopWidget desktop_widget;
    QRect screen_geometry = desktop_widget.availableGeometry();
    move(screen_geometry.width() / 2 - width() / 2, screen_geometry.height() / 2 - height() / 2);

    setCentralWidget(ui_widget);
}

void JtvLiveUiMain::toggleUi()
{
    if(b_is_advanced_ui) //Load simplified GUI
    {
        clearAdvancedUi();
        b_is_advanced_ui = false;
        loadBasicUi();
    }
    else //Load advanced GUI
    {
        clearBasicUi();
        b_is_advanced_ui = true;
        loadAdvancedUi();
    }
    settings->setValue("gui/advanced", b_is_advanced_ui);
}

void JtvLiveUiMain::onGotoRecord()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_basic_rtmpdump));
}

void JtvLiveUiMain::onGotoWatchAndStart()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_watch));
    ui_watch->linkedProcessesStart();
}

void JtvLiveUiMain::loadAdvancedUi()
//You have to reflect changes made here in clearAdvancedUi()
{
    layout()->setSizeConstraint(QLayout::SetNoConstraint); //the main layout won't resize the QMainWindow
    setMinimumSize(496, 270);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    resize(596, 270);

    ui_jtv = new JtvLiveUiTabJustin_tv(settings, live_channel);
    ui_watch = new JtvLiveUiTabWatch(settings, live_channel);
    ui_params = new JtvLiveUiTabParams(live_channel);
    ui_rtmpdump = new JtvLiveUiTabRtmpdump(settings, live_channel);
    ui_rtmpgw = new JtvLiveUiTabRtmpgw(settings, live_channel);
    ui_about = new JtvLiveUiTabAbout;

    ui_widget->addTab(ui_jtv, QIcon(":img/jtv.png"), "Justin.tv");
    ui_widget->addTab(ui_watch, QIcon(":img/television.png"), "Watch");
    ui_widget->addTab(ui_params, QIcon(":img/gear.png"), "Params");
    ui_widget->addTab(ui_rtmpdump, QIcon(":img/hdd.png"), "rtmpdump");
    ui_widget->addTab(ui_rtmpgw, QIcon(":img/netdd.png"), "rtmpgw");
    ui_widget->addTab(ui_about, QIcon(":img/lightbulb.png"), "About");

    connect(ui_jtv, SIGNAL(askClearParams()), ui_params, SLOT(clearParams()));
    connect(ui_jtv, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    connect(ui_jtv, SIGNAL(toggleUi()), this, SLOT(toggleUi()));
    connect(ui_watch, SIGNAL(askBtn_watchEnable()), ui_jtv, SLOT(btn_watchEnable()));
    connect(ui_watch, SIGNAL(askBtn_watchDisable()), ui_jtv, SLOT(btn_watchDisable()));
}

void JtvLiveUiMain::clearAdvancedUi()
{
    ui_widget->clear();

    disconnect(ui_jtv, SIGNAL(askClearParams()), ui_params, SLOT(clearParams()));
    disconnect(ui_jtv, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    disconnect(ui_jtv, SIGNAL(toggleUi()), this, SLOT(toggleUi()));
    disconnect(ui_watch, SIGNAL(askBtn_watchEnable()), ui_jtv, SLOT(btn_watchEnable()));
    disconnect(ui_watch, SIGNAL(askBtn_watchDisable()), ui_jtv, SLOT(btn_watchDisable()));

    delete ui_jtv;
    delete ui_watch;
    delete ui_params;
    delete ui_rtmpdump;
    delete ui_rtmpgw;
    delete ui_about;
}

void JtvLiveUiMain::loadBasicUi()
//You have to reflect changes made here in clearBasicUi()
{
    setFixedSize(400, 250);

    ui_basic_jtv = new JtvLiveUiTabBasicJustin_tv(settings, live_channel);
    ui_watch = new JtvLiveUiTabWatch(settings, live_channel);
    ui_basic_rtmpdump = new JtvLiveUiTabBasicRtmpdump(settings, live_channel);
    ui_about = new JtvLiveUiTabAbout;

    ui_widget->addTab(ui_basic_jtv, QIcon(":img/jtv.png"), "Justin.tv");
    ui_widget->addTab(ui_watch, QIcon(":img/television.png"), "Watch");
    ui_widget->addTab(ui_basic_rtmpdump, QIcon(":img/hdd.png"), "Record");
    ui_widget->addTab(ui_about, QIcon(":img/lightbulb.png"), "About");

    connect(ui_basic_jtv, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    connect(ui_basic_jtv, SIGNAL(gotoRecord()), this, SLOT(onGotoRecord()));
    connect(ui_basic_jtv, SIGNAL(toggleUi()), this, SLOT(toggleUi()));
    connect(ui_watch, SIGNAL(askBtn_watchEnable()), ui_basic_jtv, SLOT(btn_watchEnable()));
    connect(ui_watch, SIGNAL(askBtn_watchDisable()), ui_basic_jtv, SLOT(btn_watchDisable()));
}

void JtvLiveUiMain::clearBasicUi()
{
    ui_widget->clear();

    disconnect(ui_basic_jtv, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    disconnect(ui_basic_jtv, SIGNAL(gotoRecord()), this, SLOT(onGotoRecord()));
    disconnect(ui_basic_jtv, SIGNAL(toggleUi()), this, SLOT(toggleUi()));
    disconnect(ui_watch, SIGNAL(askBtn_watchEnable()), ui_basic_jtv, SLOT(btn_watchEnable()));
    disconnect(ui_watch, SIGNAL(askBtn_watchDisable()), ui_basic_jtv, SLOT(btn_watchDisable()));

    delete ui_basic_jtv;
    delete ui_watch;
    delete ui_basic_rtmpdump;
    delete ui_about;
}

JtvLiveUiMain::~JtvLiveUiMain()
{ }
