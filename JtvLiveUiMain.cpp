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

        //Tab 0 : Justin.tv
    ui_tab0 = new JtvLiveUiTabJustin_tv(live_channel);

        //Tab 1 : Parameters
    ui_tab1 = new JtvLiveUiTabParams(live_channel);

    ui_tab3 = new JtvLiveUiTabWatch(settings, live_channel);

        //Tab 2 : rtmpdump
    ui_tab2 = new JtvLiveUiTabRtmpdump(settings, live_channel);

        //Tab 4 : rtmpgw
    ui_tab4 = new JtvLiveUiTabRtmpgw(settings, live_channel);

    //Tab 5 : About
    ui_tab5 = new JtvLiveUiTabAbout;

        //Update Tab
        ui_tabUpdate = new QWidget(this);
        ui_tabUpdate_notice = new QLabel;
        ui_tabUpdate_notice->setOpenExternalLinks(true);
        ui_tabUpdate_notes = new QPlainTextEdit;
        ui_tabUpdate_notes->setReadOnly(true);
        ui_tabUpdate_layout = new QVBoxLayout;
        ui_tabUpdate_layout->addWidget(ui_tabUpdate_notice);
        ui_tabUpdate_layout->addWidget(ui_tabUpdate_notes);
        ui_tabUpdate->setLayout(ui_tabUpdate_layout);

    //QTabWidget setup
    ui_widget->addTab(ui_tab0, "Justin.tv");
    ui_widget->addTab(ui_tab3, "Watch");
    ui_widget->addTab(ui_tab1, "Params");
    ui_widget->addTab(ui_tab2, "rtmpdump");
    ui_widget->addTab(ui_tab4, "rtmpgw");
    ui_widget->addTab(ui_tab5, "About");

    //Central signals/slots
    connect(ui_tab0, SIGNAL(askClearParams()), ui_tab1, SLOT(clearParams()));
    connect(ui_tab0, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    connect(ui_tab3, SIGNAL(askBtn_watchEnable()), ui_tab0, SLOT(btn_watchEnable()));
    connect(ui_tab3, SIGNAL(askBtn_watchDisable()), ui_tab0, SLOT(btn_watchDisable()));

    connect(updater, SIGNAL(updateAvailable(const QString &, const QString &)), this, SLOT(TabUpdate_show(const QString &, const QString &)));
    connect(updater, SIGNAL(updateNotes(const QString &)), ui_tabUpdate_notes, SLOT(setPlainText(const QString &)));

    setCentralWidget(ui_widget);
}

void JtvLiveUiMain::onGotoWatchAndStart()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_tab3));
    ui_tab3->linkedProcessesStart();
}

//Update Tab slots
void JtvLiveUiMain::TabUpdate_show(const QString &new_version_human, const QString &dl_link)
{
    QString notice = QString("Update %1 is available. <a href=\"%2\">%2</a>").arg(new_version_human, dl_link);
    ui_tabUpdate_notice->setText(notice);
    ui_widget->addTab(ui_tabUpdate, QString("A new update is available! (v. %1)").arg(new_version_human));
    //ui_widget->tabBar()->setTabTextColor(ui_widget->indexOf(ui_tabUpdate), QColor(255, 127, 13)); -> NOPE!
}

JtvLiveUiMain::~JtvLiveUiMain()
{ }
