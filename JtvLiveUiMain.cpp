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
#ifdef Q_OS_WIN32
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

    //Hosted QProcesses setup
    linkedProcess_rtmpgw = new QProcess(this);
    linkedProcess_rtmpgw->setProcessChannelMode(QProcess::MergedChannels);
    linkedProcess_player = new QProcess(this);
    linkedProcess_player->setProcessChannelMode(QProcess::MergedChannels);

    //Core connections
    connect(live_channel, SIGNAL(messageChanged(const QString &)), this, SLOT(Tab0_onMessageChanged(const QString &)));
    connect(live_channel, SIGNAL(channelSearchSuccess(QList<JtvLiveStream> *)), this, SLOT(Tab0_onSearchSuccess(QList<JtvLiveStream> *)));
    connect(live_channel, SIGNAL(channelSearchError(const QString &)), this, SLOT(Tab0_onSearchError(const QString &)));

    /* GUI */
    setWindowTitle("Justin.tv live downloader");

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
        ui_tab0 = new QWidget;

        //ui_tab0_chanLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_tab0_channel = new QLineEdit;
        ui_tab0_chanLabel = new QLabel("Channel name :");
        ui_tab0_chanLabel->setBuddy(ui_tab0_channel);
        ui_tab0_favouriteBtn = new QPushButton("F");
        ui_tab0_favouriteBtn->setToolTip("favourite: TODO");
        ui_tab0_favouriteBtn->setDisabled(true);
        //ui_tab0_favouriteBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        ui_tab0_password = new QLineEdit;
        ui_tab0_passwdLabel = new QLabel("Channel password :");
        ui_tab0_passwdLabel->setBuddy(ui_tab0_password);
        ui_tab0_searchBtn = new QSIRPushButton(":img/zoom.png");
        //ui_tab0_searchBtn->resize(10, 10);
        //ui_tab0_searchBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::QSizePolicy::Preferred);
        //ui_tab0_searchBtn->setFixedSize(100, 100);
        /*ui_tab0_searchBtn->setIcon(QIcon(":img/zoom.png"));
        ui_tab0_searchBtn->setToolTip("Search the channel");*/



        ui_tab0_parsingInfos = new QLabel("First, type the channel name in the field above and the password if any.");
        ui_tab0_parsingInfos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_tab0_hSeparator = new QFrame;
        ui_tab0_hSeparator->setFrameShape(QFrame::HLine);
        ui_tab0_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_tab0_streamSelector = new QComboBox;
        ui_tab0_streamSelector->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        ui_tab0_streamSelector->setDisabled(true);
        ui_tab0_gotoWatch = new QPushButton;
        ui_tab0_gotoWatch->setIcon(QIcon(":img/television.png"));
        ui_tab0_gotoWatch->setToolTip("Watch this stream");
        ui_tab0_gotoWatch->setDisabled(true);
        ui_tab0_bitrate = new QLabel;
        ui_tab0_bitrate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_tab0_viewers = new QLabel;
        ui_tab0_viewers->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_tab0_part = new QLabel;
        ui_tab0_part->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_tab0_id = new QLabel;
        ui_tab0_id->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_tab0_node = new QLabel;
        ui_tab0_node->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        Tab0_defaultStats();
        //Layouts
        /*ui_tab0_nameFieldLayout = new QHBoxLayout;
        ui_tab0_nameFieldLayout->addWidget(ui_tab0_channel);
        ui_tab0_nameFieldLayout->addWidget(ui_tab0_favouriteBtn);
        ui_tab0_formLayout = new QFormLayout;
        ui_tab0_formLayout->addRow("Channel name :", ui_tab0_nameFieldLayout);
        ui_tab0_formLayout->addRow("Channel password :", ui_tab0_password);
        ui_tab0_searchLayout = new QHBoxLayout;
        ui_tab0_searchLayout->addLayout(ui_tab0_formLayout);
        ui_tab0_searchLayout->addWidget(ui_tab0_searchBtn);*/

        /*ui_tab0_searchLayout = new QHBoxLayout;
        ui_tab0_searchLayout->addWidget(ui_tab0_chanName);
        ui_tab0_searchLayout->addWidget(ui_tab0_channel);
        ui_tab0_searchLayout->addWidget(ui_tab0_searchBtn);
        ui_tab0_passwdLayout = new QHBoxLayout;
        ui_tab0_passwdLayout->addWidget(ui_tab0_chanPass);
        ui_tab0_passwdLayout->addWidget(ui_tab0_password);*/

        ui_tab0_searchLayout = new QGridLayout;
        ui_tab0_searchLayout->addWidget(ui_tab0_chanLabel, 0, 0);
        ui_tab0_searchLayout->addWidget(ui_tab0_channel, 0, 1);
        ui_tab0_searchLayout->addWidget(ui_tab0_favouriteBtn, 0, 2);
        ui_tab0_searchLayout->addWidget(ui_tab0_passwdLabel, 1, 0);
        ui_tab0_searchLayout->addWidget(ui_tab0_password, 1, 1, 1, 2);
        ui_tab0_searchLayout->addWidget(ui_tab0_searchBtn, 0, 3, 2, 1);

        ui_tab0_streamLayout = new QHBoxLayout;
        ui_tab0_streamLayout->addWidget(ui_tab0_streamSelector);
        ui_tab0_streamLayout->addWidget(ui_tab0_gotoWatch);

        ui_tab0_statsLayout = new QGridLayout;
        ui_tab0_statsLayout->addWidget(ui_tab0_bitrate, 0, 0);
        ui_tab0_statsLayout->addWidget(ui_tab0_viewers, 0, 1);
        ui_tab0_statsLayout->addWidget(ui_tab0_part, 0, 2);
        ui_tab0_statsLayout->addWidget(ui_tab0_id, 1, 0);
        ui_tab0_statsLayout->addWidget(ui_tab0_node, 1, 1);

        ui_tab0_layout = new QVBoxLayout();
        ui_tab0_layout->addLayout(ui_tab0_searchLayout);
        ui_tab0_layout->addWidget(ui_tab0_parsingInfos);
        ui_tab0_layout->addWidget(ui_tab0_hSeparator);
        ui_tab0_layout->addLayout(ui_tab0_streamLayout);
        ui_tab0_layout->addLayout(ui_tab0_statsLayout);
        //////////////////////////////////
        /*ui_tab0_statsLayout = new QGridLayout;
        ui_tab0_layout->addLayout(ui_tab0_searchLayout, 1, 0, 1, 3);
        //ui_tab0_layout->addLayout(ui_tab0_passwdLayout, 1, 0, 1, 3);
        ui_tab0_layout->addWidget(ui_tab0_parsingInfos, 2, 0, 1, 3);
        ui_tab0_layout->addWidget(ui_tab0_hSeparator, 3, 0, 1, 3);
        ui_tab0_layout->addLayout(ui_tab0_streamLayout, 4, 0, 1, 3);
        ui_tab0_statsLayout->addWidget(ui_tab0_bitrate, 5, 0);
        ui_tab0_statsLayout->addWidget(ui_tab0_viewers, 5, 1);
        ui_tab0_statsLayout->addWidget(ui_tab0_part, 5, 2);
        ui_tab0_statsLayout->addWidget(ui_tab0_id, 6, 0);
        ui_tab0_statsLayout->addWidget(ui_tab0_node, 6, 1);*/
        ui_tab0->setLayout(ui_tab0_layout);

        //Tab 1 : Parameters
        ui_tab1 = new QWidget;
        ui_tab1_rtmp = new QLineEdit;
        ui_tab1_l_rtmp = new QLabel("RTMP (-r)");
        ui_tab1_l_rtmp->setBuddy(ui_tab1_rtmp);
        ui_tab1_swf = new QLineEdit;
        ui_tab1_l_swf = new QLabel("SWF (-s)");
        ui_tab1_l_swf->setBuddy(ui_tab1_swf);
        ui_tab1_swfVfy = new QLineEdit;
        ui_tab1_l_swfVfy = new QLabel("SWF verif (-W)");
        ui_tab1_l_swfVfy->setBuddy(ui_tab1_swfVfy);
        ui_tab1_web = new QLineEdit;
        ui_tab1_l_web = new QLabel("Web page (-p)");
        ui_tab1_l_web->setBuddy(ui_tab1_web);
        ui_tab1_usherToken = new QLineEdit;
        ui_tab1_l_usherToken = new QLabel("UsherToken (-j)");
        ui_tab1_l_usherToken->setBuddy(ui_tab1_usherToken);
        ui_tab1_hSeparator = new QFrame;
        ui_tab1_hSeparator->setFrameShape(QFrame::HLine);
        ui_tab1_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_tab1_l_cliFriendly = new QLabel("Command line input firendly :");
        ui_tab1_cliFriendly = new QPlainTextEdit;
        ui_tab1_cliFriendly->setReadOnly(true);
        //Layout
        ui_tab1_layout = new QGridLayout;
        ui_tab1_layout->addWidget(ui_tab1_l_rtmp, 0, 0);
        ui_tab1_layout->addWidget(ui_tab1_rtmp, 0, 1);
        ui_tab1_layout->addWidget(ui_tab1_l_swf, 1, 0);
        ui_tab1_layout->addWidget(ui_tab1_swf, 1, 1);
        ui_tab1_layout->addWidget(ui_tab1_l_swfVfy, 2, 0);
        ui_tab1_layout->addWidget(ui_tab1_swfVfy, 2, 1);
        ui_tab1_layout->addWidget(ui_tab1_l_web, 3, 0);
        ui_tab1_layout->addWidget(ui_tab1_web, 3, 1);
        ui_tab1_layout->addWidget(ui_tab1_l_usherToken, 4, 0);
        ui_tab1_layout->addWidget(ui_tab1_usherToken, 4, 1);
        ui_tab1_layout->addWidget(ui_tab1_hSeparator, 5, 0, 1, 2);
        ui_tab1_layout->addWidget(ui_tab1_l_cliFriendly, 6, 0, 1, 2);
        ui_tab1_layout->addWidget(ui_tab1_cliFriendly, 7, 0, 1, 2);
        ui_tab1->setLayout(ui_tab1_layout);

        //Tab 2 : rtmpdump
        ui_tab2 = new QWidget;
        ui_tab2_file_box = new QGroupBox("Outfile :");
        ui_tab2_file_box->setCheckable(true);
        ui_tab2_file_box->setChecked(true);
        ui_tab2_file = new QLineEdit;
        ui_tab2_file_btn = new QPushButton("...");
        ui_tab2_file_layout = new QHBoxLayout();
        ui_tab2_file_layout->addWidget(ui_tab2_file);
        ui_tab2_file_layout->addWidget(ui_tab2_file_btn);
        ui_tab2_file_box->setLayout(ui_tab2_file_layout);
        ui_tab2_pipe_box = new QGroupBox("Pipe :");
        ui_tab2_pipe_box->setCheckable(true);
        ui_tab2_pipe_box->setChecked(false);
        ui_tab2_pipe = new QLineEdit;
        ui_tab2_pipe->setToolTip("Right side of the pipe (after | )");
        ui_tab2_pipe->setText(settings->value("rtmpdump/pipe").toString());
        ui_tab2_pipe_layout = new QHBoxLayout;
        ui_tab2_pipe_layout->addWidget(ui_tab2_pipe);
        ui_tab2_pipe_box->setLayout(ui_tab2_pipe_layout);
        ui_tab2_verbosity_box = new QGroupBox("Verbosity :");
        ui_tab2_verbosity_normal = new QRadioButton("Normal");
        ui_tab2_verbosity_normal->setChecked(true);
        ui_tab2_verbosity_verbose = new QRadioButton("Verbose");
        ui_tab2_verbosity_debug = new QRadioButton("Debug");
        ui_tab2_verbosity_layout = new QHBoxLayout;
        ui_tab2_verbosity_layout->addWidget(ui_tab2_verbosity_normal);
        ui_tab2_verbosity_layout->addWidget(ui_tab2_verbosity_verbose);
        ui_tab2_verbosity_layout->addWidget(ui_tab2_verbosity_debug);
        ui_tab2_verbosity_box->setLayout(ui_tab2_verbosity_layout);
        ui_tab2_start = new QPushButton("Start");
        //Layout
        ui_tab2_layout = new QVBoxLayout;
        ui_tab2_layout->addWidget(ui_tab2_file_box);
        ui_tab2_layout->addWidget(ui_tab2_pipe_box);
        ui_tab2_layout->addWidget(ui_tab2_verbosity_box);
        ui_tab2_layout->addWidget(ui_tab2_start);
        ui_tab2->setLayout(ui_tab2_layout);

        //Tab 3 : Watch
        ui_tab3 = new QWidget;
        ui_tab3_player = new QLineEdit;
#ifdef Q_OS_WIN32
        ui_tab3_player_label = new QLabel("Player path :");
        ui_tab3_player->setText(settings->value("watch/player", "%programfiles%\\VideoLAN\\VLC\\vlc.exe").toString());
#else
        ui_tab3_player_label = new QLabel("Player command :");
        ui_tab3_player->setText(settings->value("watch/player", "vlc").toString());
#endif
        ui_tab3_player_layout = new QHBoxLayout;
        ui_tab3_player_layout->addWidget(ui_tab3_player_label);
        ui_tab3_player_layout->addWidget(ui_tab3_player);
        ui_tab3_watchBtn = new QPushButton("Watch");
        ui_tab3_watchBtn->setIcon(QIcon(":img/television.png"));
        ui_tab3_hSeparator = new QFrame;
        ui_tab3_hSeparator->setFrameShape(QFrame::HLine);
        ui_tab3_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_tab3_rtmpgwOut = new QPlainTextEdit;
        ui_tab3_rtmpgwOut->setReadOnly(true);
        ui_tab3_rtmpgwOut->setOverwriteMode(true);
        ui_tab3_playerOut = new QPlainTextEdit;
        ui_tab3_playerOut->setReadOnly(true);
        //Layout
        ui_tab3_layout = new QVBoxLayout;
        ui_tab3_layout->addLayout(ui_tab3_player_layout);
        ui_tab3_layout->addWidget(ui_tab3_watchBtn);
        ui_tab3_layout->addWidget(ui_tab3_hSeparator);
        ui_tab3_layout->addWidget(ui_tab3_rtmpgwOut);
        ui_tab3_layout->addWidget(ui_tab3_playerOut);
        ui_tab3->setLayout(ui_tab3_layout);

        //Tab 4 : rtmpgw
        ui_tab4 = new QWidget;
        ui_tab4_params_box = new QGroupBox("Parameters :");
        ui_tab4_params_l_ip = new QLabel("Binding address :");
        ui_tab4_params_l_port = new QLabel("Listening port :");
        ui_tab4_params_colon = new QLabel(":");
        ui_tab4_params_ip = new QLineEdit;
        ui_tab4_params_ip->setText(settings->value("rtmpgw/ip", "0.0.0.0").toString());
        ui_tab4_params_port = new QSpinBox;
        ui_tab4_params_port->setRange(1,65335);
        ui_tab4_params_port->setValue(settings->value("rtmpgw/port", 80).toInt());
        ui_tab4_params_layout = new QGridLayout;
        ui_tab4_params_layout->addWidget(ui_tab4_params_l_ip, 0, 0);
        ui_tab4_params_layout->addWidget(ui_tab4_params_l_port, 0, 2);
        ui_tab4_params_layout->addWidget(ui_tab4_params_ip, 1, 0);
        ui_tab4_params_layout->addWidget(ui_tab4_params_colon, 1, 1);
        ui_tab4_params_layout->addWidget(ui_tab4_params_port, 1, 2);
        ui_tab4_params_box->setLayout(ui_tab4_params_layout);
        ui_tab4_verbosity_box = new QGroupBox("Verbosity :");
        ui_tab4_verbosity_normal = new QRadioButton("Normal");
        ui_tab4_verbosity_normal->setChecked(true);
        ui_tab4_verbosity_verbose = new QRadioButton("Verbose");
        ui_tab4_verbosity_debug = new QRadioButton("Debug");
        ui_tab4_verbosity_layout = new QHBoxLayout;
        ui_tab4_verbosity_layout->addWidget(ui_tab4_verbosity_normal);
        ui_tab4_verbosity_layout->addWidget(ui_tab4_verbosity_verbose);
        ui_tab4_verbosity_layout->addWidget(ui_tab4_verbosity_debug);
        ui_tab4_verbosity_box->setLayout(ui_tab4_verbosity_layout);
        ui_tab4_start = new QPushButton("Start");
        //Layout
        ui_tab4_layout = new QVBoxLayout;
        ui_tab4_layout->addWidget(ui_tab4_params_box);
        ui_tab4_layout->addWidget(ui_tab4_verbosity_box);
        ui_tab4_layout->addWidget(ui_tab4_start);
        ui_tab4->setLayout(ui_tab4_layout);

        //Tab 5 : About
        ui_tab5 = new QWidget;
        ui_tab5_copyrightNotice = new QLabel(QString("<p align=\"center\"><b>Jtv live downloader v. %1</b><br />Copyright © 2012 toine512<br />Compiled on [%2] [%3]<br /><br />This software is distributed under <a href=\"https://www.gnu.org/licenses/gpl.html\">GNU General Public License v. 3</a>.</p><p>Written in C++ using <a href=\"https://qt.nokia.com/\">Qt</a> 4.7.4 (<a href=\"https://www.gnu.org/licenses/gpl.html\">GNU GPL v. 3</a>)<br />Uses <a href=\"http://www.famfamfam.com/lab/icons/silk/\">FAMFAMFAM Silk Icons</a> by <a href=\"http://www.famfamfam.com/\">Mark James</a> (<a href=\"https://creativecommons.org/licenses/by/2.5/\">CC BY 2.5</a>).</p>").arg(JTV_LIVE_VERSION_HUMAN, __DATE__, __TIME__));
        ui_tab5_copyrightNotice->setOpenExternalLinks(true);
        ui_tab5_gplv3 = new QLabel;
        ui_tab5_gplv3->setPixmap(QPixmap(":img/gplv3.png"));
        ui_tab5_aboutQt = new QPushButton("About Qt");
        //Layout
        ui_tab5_layout = new QGridLayout;
        ui_tab5_layout->addWidget(ui_tab5_copyrightNotice, 0, 0, 1, 2);
        ui_tab5_layout->addWidget(ui_tab5_gplv3, 1, 0, 1, 1);
        ui_tab5_layout->addWidget(ui_tab5_aboutQt, 1, 1, 1, 1);
        ui_tab5->setLayout(ui_tab5_layout);

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
    connect(ui_tab0_channel, SIGNAL(returnPressed()), this, SLOT(Tab0_searchChannel()));
    connect(ui_tab0_password, SIGNAL(returnPressed()), this, SLOT(Tab0_searchChannel()));
    connect(ui_tab0_searchBtn, SIGNAL(clicked()), this, SLOT(Tab0_searchChannel()));
    connect(ui_tab0_streamSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(Tab0_updateStreamDatas(int)));
    connect(ui_tab0_gotoWatch, SIGNAL(clicked()), this, SLOT(Tab0_gotoWatchAndStart()));
    connect(ui_tab1_rtmp, SIGNAL(textEdited(QString)), this, SLOT(Tab1_buildCliFriendly()));
    connect(ui_tab1_swf, SIGNAL(textEdited(QString)), this, SLOT(Tab1_buildCliFriendly()));
    connect(ui_tab1_swfVfy, SIGNAL(textEdited(QString)), this, SLOT(Tab1_buildCliFriendly()));
    connect(ui_tab1_web, SIGNAL(textEdited(QString)), this, SLOT(Tab1_buildCliFriendly()));
    connect(ui_tab1_usherToken, SIGNAL(textEdited(QString)), this, SLOT(Tab1_buildCliFriendly()));
    connect(ui_tab2_file_btn, SIGNAL(clicked()), this, SLOT(Tab2_browseFile()));
    connect(ui_tab2_pipe_box, SIGNAL(toggled(bool)), this, SLOT(Tab2_toggleFileCheck(bool)));
    connect(ui_tab2_file_box, SIGNAL(toggled(bool)), this, SLOT(Tab2_togglePipeCheck(bool)));
    connect(ui_tab2_pipe, SIGNAL(textEdited(const QString &)), this, SLOT(Tab2_savePipe(const QString &)));
    connect(ui_tab2_start, SIGNAL(clicked()), this, SLOT(Tab2_startRtmpdump()));
    connect(ui_tab3_player, SIGNAL(textEdited(const QString &)), this, SLOT(Tab3_savePlayerPath(const QString &)));
    connect(ui_tab3_watchBtn, SIGNAL(clicked()), this, SLOT(Tab3_linkedProcessesStart()));
    connect(ui_tab4_params_ip, SIGNAL(textEdited(const QString &)), this, SLOT(Tab4_saveIp(const QString &)));
    connect(ui_tab4_params_port, SIGNAL(valueChanged(int)), this, SLOT(Tab4_savePort(int)));
    connect(ui_tab4_start, SIGNAL(clicked()), this, SLOT(Tab4_startRtmpgw()));
    connect(ui_tab5_aboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
    connect(updater, SIGNAL(updateAvailable(const QString &, const QString &)), this, SLOT(TabUpdate_show(const QString &, const QString &)));
    connect(updater, SIGNAL(updateNotes(const QString &)), ui_tabUpdate_notes, SLOT(setPlainText(const QString &)));
    connect(linkedProcess_rtmpgw, SIGNAL(readyReadStandardOutput()), this, SLOT(Tab3_rtmpgwOut()));
    connect(linkedProcess_player, SIGNAL(readyReadStandardOutput()), this, SLOT(Tab3_playerOut()));

    setCentralWidget(ui_widget);
}

//Tab 0 slots
void JtvLiveUiMain::Tab0_searchChannel()
{
    if(ui_tab0_channel->text().isEmpty())
    {
        QMessageBox::information(this, "Search live channel", "Empty channel name field");
    }
    else
    {
        Tab0_defaultStats();
        Tab0_lock();
        ui_tab0_streamSelector->setDisabled(true);
        ui_tab0_streamSelector->clear();
        ui_tab0_gotoWatch->setDisabled(true);
        Tab1_defaultParams();
        live_channel->startSearch(ui_tab0_channel->text(), ui_tab0_password->text());
    }
}

void JtvLiveUiMain::Tab0_onMessageChanged(const QString &message)
{
    ui_tab0_parsingInfos->setText(message);
}

void JtvLiveUiMain::Tab0_onSearchSuccess(QList<JtvLiveStream> *streams)
{
    int s = streams->size();
    for(int i = 0 ; i < s ; i++)
    {
        QString name = QString("%1\t\tHeight : %2\t\t").arg(streams->at(i).tag_name, streams->at(i).height);
        if(streams->at(i).server_type == JtvLiveStream::UsherServer)
        {
            name.append("UsherToken");
        }
        else if(streams->at(i).server_type == JtvLiveStream::AkamaiServer)
        {
            name.append("SWF Vfy");
        }
        ui_tab0_streamSelector->addItem(name);
    }
    ui_tab0_streamSelector->setCurrentIndex(0); //Will call Tab0_updateStreamDatas(int) [slot]
    ui_tab0_streamSelector->setEnabled(true);
    ui_tab0_gotoWatch->setEnabled(true);
    Tab0_unlock();
}

void JtvLiveUiMain::Tab0_onSearchError(const QString &error)
{
    QMessageBox::warning(this, "Search live channel", QString("An error occured : %1").arg(error));
    Tab0_unlock();
}

void JtvLiveUiMain::Tab0_updateStreamDatas(int index)
{
    Tab0_defaultStats();
    Tab1_defaultParams();
    if(index >= 0) //Avoiding crash when the QComboBox is cleared
    {
        const JtvLiveStream &stream = live_channel->getStreams()->at(index);
        Tab0_fillStats(stream);
        Tab1_fillParams(stream);
    }
}

void JtvLiveUiMain::Tab0_gotoWatchAndStart()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_tab3));
    Tab3_linkedProcessesStart();
}

//Tab 0 protected
void JtvLiveUiMain::Tab0_lock()
{
    ui_tab0_channel->setDisabled(true);
    ui_tab0_searchBtn->setDisabled(true);
    ui_tab0_password->setDisabled(true);
}

void JtvLiveUiMain::Tab0_unlock()
{
    ui_tab0_channel->setEnabled(true);
    ui_tab0_searchBtn->setEnabled(true);
    ui_tab0_password->setEnabled(true);
}

void JtvLiveUiMain::Tab0_defaultStats()
{
    ui_tab0_bitrate->setText("bitrate : n/a");
    ui_tab0_viewers->setText("viewers : n/a");
    ui_tab0_part->setText("part : n/a");
    ui_tab0_id->setText("id : n/a");
    ui_tab0_node->setText("node : n/a");
}

void JtvLiveUiMain::Tab0_fillStats(const JtvLiveStream &stream)
{
    ui_tab0_bitrate->setText(QString("bitrate : ").append(stream.bitrate));
    ui_tab0_viewers->setText(QString("viewers : ").append(stream.viewers));
    ui_tab0_part->setText(QString("part : ").append(stream.part));
    ui_tab0_id->setText(QString("id : ").append(stream.id));
    ui_tab0_node->setText(QString("node : ").append(stream.node));
}

//Tab 1 slot
void JtvLiveUiMain::Tab1_buildCliFriendly()
{
    ui_tab1_cliFriendly->setPlainText(QString(getCommandEscaped(collectRtmpParams())).append(" -v"));
}

//Tab 1 protected
void JtvLiveUiMain::Tab1_defaultParams()
{
    ui_tab1_rtmp->clear();
    ui_tab1_swf->clear();
    ui_tab1_web->clear();
    ui_tab1_swfVfy->clear();
    ui_tab1_usherToken->clear();
    ui_tab1_cliFriendly->clear();
}

void JtvLiveUiMain::Tab1_fillParams(const JtvLiveStream &stream)
{
    ui_tab1_rtmp->setText(stream.rtmp_url);
    ui_tab1_swf->setText(live_channel->getPlayerUrl());
    ui_tab1_web->setText(QString(live_channel->getHttpReferer()).append(stream.channel_name));
    if(stream.server_type == JtvLiveStream::UsherServer)
    {
        ui_tab1_usherToken->setText(stream.usher_token);
    }
    else if(stream.server_type == JtvLiveStream::AkamaiServer)
    {
        ui_tab1_swfVfy->setText(live_channel->getPlayerUrl());
    }
    Tab1_buildCliFriendly();
}

//Tab 2 slots
void JtvLiveUiMain::Tab2_browseFile()
{
    ui_tab2_file->setText(QFileDialog::getSaveFileName(this, "Save stream", QString(), "Flash video (*.flv)"));
}

//Called by ui_tab2_pipe_box
void JtvLiveUiMain::Tab2_toggleFileCheck(bool pipe_ckecked)
{
    if(pipe_ckecked)
    {
        ui_tab2_file_box->setChecked(false);
    }
    else
    {
        ui_tab2_file_box->setChecked(true);
    }
}

//Called by ui_tab2_file_box
void JtvLiveUiMain::Tab2_togglePipeCheck(bool file_ckecked)
{
    if(file_ckecked)
    {
        ui_tab2_pipe_box->setChecked(false);
    }
    else
    {
        ui_tab2_pipe_box->setChecked(true);
    }
}

void JtvLiveUiMain::Tab2_savePipe(const QString &text)
{
    settings->setValue("rtmpdump/pipe", text);
}

void JtvLiveUiMain::Tab2_startRtmpdump()
{
    QStringList args = collectRtmpParams();
    if(args.isEmpty())
    {
        QMessageBox::warning(this, "Parameters", "RTMP parameters are empty.");
    }
    else
    {
        args << "-f";
        args << settings->value("flash/version", "WIN 11,1,102,62").toString();
        args << "-v";
        if(ui_tab2_file_box->isChecked())
        {
            if(ui_tab2_file->text().isEmpty())
            {
                QMessageBox::warning(this, "Output file", "No output path provided.");
            }
            else
            {
                if(ui_tab2_verbosity_verbose->isChecked())
                {
                    args << "-V";
                }
                else if(ui_tab2_verbosity_debug->isChecked())
                {
                    args << "-z";
                }
                args << "-o";
                args << ui_tab2_file->text();
#ifdef Q_OS_WIN32
                if(!QProcess::startDetached(settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), args))
#else
                if(!QProcess::startDetached(settings->value("terminal/terminal", "xterm").toString(), QStringList() << settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2").arg(settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), getCommandEscaped(args).replace("&", "\\&")))) //"&" is meaningful under Linux ;) -> replace("&", "\\&")
#endif
                {
                    QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
                }
            }
        }
        else if(ui_tab2_pipe_box->isChecked())
        {
            if(ui_tab2_pipe->text().isEmpty())
            {
                QMessageBox::warning(this, "Piping", "Right side of the pipe is empty.");
            }
            else
            {
                args << "-q";
#ifdef Q_OS_WIN32
                if(!QProcess::startDetached(QString("cmd.exe /c \"%1 %2 | %3\"").arg(settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), getCommandEscaped(args).replace("\\\"", "\"\"\""), ui_tab2_pipe->text()))) //epic Windows crap ! -> replace("\\\"", "\"\"\"")
#else
                if(!QProcess::startDetached(settings->value("terminal/terminal", "xterm").toString(), QStringList() << settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2 | %3").arg(settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), getCommandEscaped(args).replace("&", "\\&"), ui_tab2_pipe->text()))) //"&" is meaningful under Linux ;) -> replace("&", "\\&")
#endif
                {
                    QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
                }
            }
        }
    }
}

//Tab 3 slots
void JtvLiveUiMain::Tab3_savePlayerPath(const QString &path)
{
    settings->setValue("watch/player", path);
}

void JtvLiveUiMain::Tab3_linkedProcessesStart()
{
    ui_tab0_gotoWatch->setDisabled(true);
    ui_tab3_watchBtn->setDisabled(true);
    if(ui_tab3_player->text().isEmpty())
    {
        QMessageBox::warning(this, "Player", "No player path/command provided.");
        ui_tab0_gotoWatch->setEnabled(true);
        ui_tab3_watchBtn->setEnabled(true);
    }
    else
    {
        QStringList args = collectRtmpParams();
        if(args.isEmpty())
        {
            QMessageBox::warning(this, "Parameters", "RTMP parameters are empty.");
            ui_tab0_gotoWatch->setEnabled(true);
            ui_tab3_watchBtn->setEnabled(true);
        }
        else
        {
            args << "-g";
            args << settings->value("watch/port", "21080").toString();
            args << "-f";
            args << settings->value("flash/version", "WIN 11,1,102,62").toString();
            args << "-v";
            //args << QString("-V");
            connect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
            connect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
            connect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
            connect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
#ifdef Q_OS_WIN32
            linkedProcess_rtmpgw->start(settings->value("watch/rtmpgw", "rtmpgw.exe").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#else
            linkedProcess_rtmpgw->start(settings->value("watch/rtmpgw", "rtmpgw").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#endif
            linkedProcess_player->start(ui_tab3_player->text(), QStringList(QString("http://127.0.0.1:").append(settings->value("watch/port", "21080").toString())), QIODevice::ReadOnly | QIODevice::Unbuffered);
        }
    }
}

void JtvLiveUiMain::Tab3_linkedProcessesError(const QProcess::ProcessError &error)
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
    if(error == QProcess::FailedToStart)
    {
        QMessageBox::warning(this, "Startig process", "Unable to start the process, check rtmpgw & player path and your permissions.");
    }
    else if(error == QProcess::Crashed)
    {
        QMessageBox::critical(this, "Process crash", "The process has crashed.");
    }
    else if(error == QProcess::ReadError)
    {
        QMessageBox::critical(this, "Process read error", "Unable to read process output.");
    }
    else
    {
        QMessageBox::critical(this, "Process error", "An error has occured.");
    }
    Tab3_linkedProcessesTerminate();
}

void JtvLiveUiMain::Tab3_linkedProcessesDisconnectTerminate()
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Tab3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Tab3_linkedProcessesDisconnectTerminate()));
    Tab3_linkedProcessesTerminate();
}

void JtvLiveUiMain::Tab3_rtmpgwOut()
{
    ui_tab3_rtmpgwOut->appendPlainText(linkedProcess_rtmpgw->readAll());
}

void JtvLiveUiMain::Tab3_playerOut()
{
    ui_tab3_playerOut->appendPlainText(linkedProcess_player->readAll());
}

//Tab 3 protected
void JtvLiveUiMain::Tab3_linkedProcessesTerminate()
{
#ifdef Q_OS_WIN32
    linkedProcess_rtmpgw->kill();
#else
    linkedProcess_rtmpgw->terminate();
#endif
    linkedProcess_player->terminate();
    ui_tab0_gotoWatch->setEnabled(true);
    ui_tab3_watchBtn->setEnabled(true);
}

//Tab 4 : slots
void JtvLiveUiMain::Tab4_saveIp(const QString &ip)
{
    settings->setValue("rtmpgw/ip", ip);
}

void JtvLiveUiMain::Tab4_savePort(int port)
{
    settings->setValue("rtmpgw/port", port);
}

void JtvLiveUiMain::Tab4_startRtmpgw()
{
    QStringList args = collectRtmpParams();
    if(args.isEmpty())
    {
        QMessageBox::warning(this, "Parameters", "RTMP parameters are empty.");
    }
    else
    {
        args << "-f";
        args << settings->value("flash/version", "WIN 11,1,102,62").toString();
        args << "-v";
        if(ui_tab4_params_ip->text().isEmpty())
        {
            QMessageBox::warning(this, "Binding IP", "No binding address provided.");
        }
        else
        {
            args << "-D";
            args << ui_tab4_params_ip->text();
            args << "-g";
            args << QString("%1").arg(ui_tab4_params_port->value());
            if(ui_tab4_verbosity_verbose->isChecked())
            {
                args << "-V";
            }
            else if(ui_tab4_verbosity_debug->isChecked())
            {
                args << "-z";
            }
#ifdef Q_OS_WIN32
            if(!QProcess::startDetached(settings->value("rtmpgw/rtmpgw", "rtmpgw.exe").toString(), args))
#else
            if(!QProcess::startDetached(settings->value("terminal/terminal", "xterm").toString(), QStringList() << settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2").arg(settings->value("rtmpgw/rtmpgw", "rtmpgw").toString(), getCommandEscaped(args).replace("&", "\\&"))))
#endif
            {
                QMessageBox::warning(this, "Launching rtmpgw", "Unable to create the process, check the path.");
            }
        }
    }
}

//Update Tab slots
void JtvLiveUiMain::TabUpdate_show(const QString &new_version_human, const QString &dl_link)
{
    QString notice = QString("Update %1 is available. %2").arg(new_version_human, dl_link);
    ui_tabUpdate_notice->setText(notice);
    ui_widget->addTab(ui_tabUpdate, QString("A new update is available! (v. %1)").arg(new_version_human));
    //ui_widget->tabBar()->setTabTextColor(ui_widget->indexOf(ui_tabUpdate), QColor(255, 127, 13));
}

QStringList JtvLiveUiMain::collectRtmpParams()
{
    QStringList args;
    if(!ui_tab1_rtmp->text().isEmpty())
    {
        args << "-r";
        args << ui_tab1_rtmp->text();
    }
    if(!ui_tab1_swf->text().isEmpty())
    {
        args << "-s";
        args << ui_tab1_swf->text();
    }
    if(!ui_tab1_swfVfy->text().isEmpty())
    {
        args << "-W";
        args << ui_tab1_swfVfy->text();
    }
    if(!ui_tab1_web->text().isEmpty())
    {
        args << "-p";
        args << ui_tab1_web->text();
    }
    if(!ui_tab1_usherToken->text().isEmpty())
    {
        args << "-j";
        args << ui_tab1_usherToken->text();
    }
    return args;
}

QString JtvLiveUiMain::getCommandEscaped(QStringList args)
{
    args.replaceInStrings("\"", "\\\"");
    int s = args.size();
    QString temp;
    for (int i = 0 ; i < s ; ++i)
    {
        temp = args.at(i);
        if(temp.contains(" "))
        {
            temp.prepend("\"");
            temp.append("\"");
            args.replace(i, temp);
        }
    }
    return args.join(" ");
}

void JtvLiveUiMain::aboutQt()
{
    QMessageBox::aboutQt(this);
}

JtvLiveUiMain::~JtvLiveUiMain()
{ }
