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
 * along with "TV sur PC Desktop".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JtvLiveUiMain.h"

JtvLiveUiMain::JtvLiveUiMain(QWidget *parent) :
    QMainWindow(parent)
{
    //Core
    settings = new QSettings("jtvdl.conf", QSettings::IniFormat, this);
    net_manager = new QNetworkAccessManager(this);
    live_channel = new JtvLiveChannel(net_manager, this);
    connect(live_channel, SIGNAL(messageChanged(const QString &)), this, SLOT(Page0_onMessageChanged(const QString &)));
    connect(live_channel, SIGNAL(channelSearchSuccess(QList<JtvLiveStream> *)), this, SLOT(Page0_onSearchSuccess(QList<JtvLiveStream> *)));
    connect(live_channel, SIGNAL(channelSearchError(const QString &)), this, SLOT(Page0_onSearchError(const QString &)));
    linkedProcess_rtmpgw = new QProcess(this);
    linkedProcess_rtmpgw->setProcessChannelMode(QProcess::MergedChannels);
    linkedProcess_player = new QProcess(this);
    linkedProcess_player->setProcessChannelMode(QProcess::MergedChannels);

    setWindowTitle("Justin.tv live downloader");
#ifdef Q_OS_WIN32
    setFixedSize(496, 270);
#else
    resize(496, 270);
#endif
    //Center on the current screen
    QDesktopWidget desktop_widget;
    QRect screen_geometry = desktop_widget.availableGeometry();
    move(screen_geometry.width() / 2 - width() / 2, screen_geometry.height() / 2 - height() / 2);

    //Statusbar
    /*ui_bottom_statusBar = statusBar();
    ui_bottom_statusBar->showMessage("I'm an useless status bar !");*/

    //Central zone
    ui_widget = new QTabWidget;

        //Page 0 : Justin.tv
        ui_page0 = new QWidget;
        ui_page0_chanName = new QLabel("Channel name :");
        ui_page0_chanName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_page0_channel = new QLineEdit;
        ui_page0_searchBtn = new QPushButton;
        ui_page0_searchBtn->setIcon(QIcon(":img/zoom.png"));
        ui_page0_searchBtn->setToolTip("Search the channel");
        ui_page0_parsingInfos = new QLabel("First, write the channel name in the field above.");
        ui_page0_parsingInfos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_page0_hSeparator = new QFrame;
        ui_page0_hSeparator->setFrameShape(QFrame::HLine);
        ui_page0_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_page0_streamSelector = new QComboBox;
        ui_page0_streamSelector->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        ui_page0_streamSelector->setDisabled(true);
        ui_page0_gotoWatch = new QPushButton;
        ui_page0_gotoWatch->setIcon(QIcon(":img/television.png"));
        ui_page0_gotoWatch->setToolTip("Watch this stream");
        ui_page0_gotoWatch->setDisabled(true);
        ui_page0_bitrate = new QLabel;
        ui_page0_bitrate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_page0_viewers = new QLabel;
        ui_page0_viewers->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_page0_part = new QLabel;
        ui_page0_part->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_page0_id = new QLabel;
        ui_page0_id->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_page0_node = new QLabel;
        ui_page0_node->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        Page0_defaultStats();
        //Layouts
        ui_page0_searchLayout = new QHBoxLayout;
        ui_page0_searchLayout->addWidget(ui_page0_chanName);
        ui_page0_searchLayout->addWidget(ui_page0_channel);
        ui_page0_searchLayout->addWidget(ui_page0_searchBtn);
        ui_page0_streamLayout = new QHBoxLayout;
        ui_page0_streamLayout->addWidget(ui_page0_streamSelector);
        ui_page0_streamLayout->addWidget(ui_page0_gotoWatch);
        ui_page0_layout = new QGridLayout;
        ui_page0_layout->addLayout(ui_page0_searchLayout, 0, 0, 1, 3);
        ui_page0_layout->addWidget(ui_page0_parsingInfos, 1, 0, 1, 3);
        ui_page0_layout->addWidget(ui_page0_hSeparator, 2, 0, 1, 3);
        ui_page0_layout->addLayout(ui_page0_streamLayout, 3, 0, 1, 3);
        ui_page0_layout->addWidget(ui_page0_bitrate, 4, 0);
        ui_page0_layout->addWidget(ui_page0_viewers, 4, 1);
        ui_page0_layout->addWidget(ui_page0_part, 4, 2);
        ui_page0_layout->addWidget(ui_page0_id, 5, 0);
        ui_page0_layout->addWidget(ui_page0_node, 5, 1);
        ui_page0->setLayout(ui_page0_layout);

        //Page 1 : Parameters
        ui_page1 = new QWidget;
        ui_page1_l_rtmp = new QLabel("RTMP (-r)");
        ui_page1_rtmp = new QLineEdit;
        ui_page1_l_swf = new QLabel("SWF (-s)");
        ui_page1_swf = new QLineEdit;
        ui_page1_l_swfVfy = new QLabel("SWF verif (-W)");
        ui_page1_swfVfy = new QLineEdit;
        ui_page1_l_web = new QLabel("Web page (-p)");
        ui_page1_web = new QLineEdit;
        ui_page1_l_usherToken = new QLabel("UsherToken (-j)");
        ui_page1_usherToken = new QLineEdit;
        ui_page1_hSeparator = new QFrame;
        ui_page1_hSeparator->setFrameShape(QFrame::HLine);
        ui_page1_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_page1_l_cliFriendly = new QLabel("Command line input firendly :");
        ui_page1_cliFriendly = new QPlainTextEdit;
        ui_page1_cliFriendly->setReadOnly(true);
        //Layout
        ui_page1_layout = new QGridLayout;
        ui_page1_layout->addWidget(ui_page1_l_rtmp, 0, 0);
        ui_page1_layout->addWidget(ui_page1_rtmp, 0, 1);
        ui_page1_layout->addWidget(ui_page1_l_swf, 1, 0);
        ui_page1_layout->addWidget(ui_page1_swf, 1, 1);
        ui_page1_layout->addWidget(ui_page1_l_swfVfy, 2, 0);
        ui_page1_layout->addWidget(ui_page1_swfVfy, 2, 1);
        ui_page1_layout->addWidget(ui_page1_l_web, 3, 0);
        ui_page1_layout->addWidget(ui_page1_web, 3, 1);
        ui_page1_layout->addWidget(ui_page1_l_usherToken, 4, 0);
        ui_page1_layout->addWidget(ui_page1_usherToken, 4, 1);
        ui_page1_layout->addWidget(ui_page1_hSeparator, 5, 0, 1, 2);
        ui_page1_layout->addWidget(ui_page1_l_cliFriendly, 6, 0, 1, 2);
        ui_page1_layout->addWidget(ui_page1_cliFriendly, 7, 0, 1, 2);
        ui_page1->setLayout(ui_page1_layout);

        //Page 2 : rtmpdump
        ui_page2 = new QWidget;
        ui_page2_file_box = new QGroupBox("Outfile :");
        ui_page2_file_box->setCheckable(true);
        ui_page2_file_box->setChecked(true);
        ui_page2_file = new QLineEdit;
        ui_page2_file_btn = new QPushButton("...");
        ui_page2_file_layout = new QHBoxLayout();
        ui_page2_file_layout->addWidget(ui_page2_file);
        ui_page2_file_layout->addWidget(ui_page2_file_btn);
        ui_page2_file_box->setLayout(ui_page2_file_layout);
        ui_page2_pipe_box = new QGroupBox("Pipe :");
        ui_page2_pipe_box->setCheckable(true);
        ui_page2_pipe_box->setChecked(false);
        ui_page2_pipe = new QLineEdit;
        ui_page2_pipe->setToolTip("Right side of the pipe (after | )");
        ui_page2_pipe->setText(settings->value("rtmpdump/pipe").toString());
        ui_page2_pipe_layout = new QHBoxLayout;
        ui_page2_pipe_layout->addWidget(ui_page2_pipe);
        ui_page2_pipe_box->setLayout(ui_page2_pipe_layout);
        ui_page2_verbosity_box = new QGroupBox("Verbosity :");
        ui_page2_verbosity_normal = new QRadioButton("Normal");
        ui_page2_verbosity_normal->setChecked(true);
        ui_page2_verbosity_verbose = new QRadioButton("Verbose");
        ui_page2_verbosity_debug = new QRadioButton("Debug");
        ui_page2_verbosity_layout = new QHBoxLayout;
        ui_page2_verbosity_layout->addWidget(ui_page2_verbosity_normal);
        ui_page2_verbosity_layout->addWidget(ui_page2_verbosity_verbose);
        ui_page2_verbosity_layout->addWidget(ui_page2_verbosity_debug);
        ui_page2_verbosity_box->setLayout(ui_page2_verbosity_layout);
        ui_page2_start = new QPushButton("Start");
        //Layout
        ui_page2_layout = new QVBoxLayout;
        ui_page2_layout->addWidget(ui_page2_file_box);
        ui_page2_layout->addWidget(ui_page2_pipe_box);
        ui_page2_layout->addWidget(ui_page2_verbosity_box);
        ui_page2_layout->addWidget(ui_page2_start);
        ui_page2->setLayout(ui_page2_layout);

        //Page 3 : Watch
        ui_page3 = new QWidget;
        ui_page3_player = new QLineEdit;
#ifdef Q_OS_WIN32
        ui_page3_player_label = new QLabel("Player path :");
        ui_page3_player->setText(settings->value("watch/player", "%programfiles%\\VideoLAN\\VLC\\vlc.exe").toString());
#else
        ui_page3_player_label = new QLabel("Player command :");
        ui_page3_player->setText(settings->value("watch/player", "vlc").toString());
#endif
        ui_page3_player_layout = new QHBoxLayout;
        ui_page3_player_layout->addWidget(ui_page3_player_label);
        ui_page3_player_layout->addWidget(ui_page3_player);
        ui_page3_watchBtn = new QPushButton("Watch");
        ui_page3_watchBtn->setIcon(QIcon(":img/television.png"));
        ui_page3_hSeparator = new QFrame;
        ui_page3_hSeparator->setFrameShape(QFrame::HLine);
        ui_page3_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_page3_rtmpgwOut = new QPlainTextEdit;
        ui_page3_rtmpgwOut->setReadOnly(true);
        ui_page3_rtmpgwOut->setOverwriteMode(true);
        ui_page3_playerOut = new QPlainTextEdit;
        ui_page3_playerOut->setReadOnly(true);
        //Layout
        ui_page3_layout = new QVBoxLayout;
        ui_page3_layout->addLayout(ui_page3_player_layout);
        ui_page3_layout->addWidget(ui_page3_watchBtn);
        ui_page3_layout->addWidget(ui_page3_hSeparator);
        ui_page3_layout->addWidget(ui_page3_rtmpgwOut);
        ui_page3_layout->addWidget(ui_page3_playerOut);
        ui_page3->setLayout(ui_page3_layout);

        //Page 4 : rtmpgw
        ui_page4 = new QWidget;
        ui_page4_params_box = new QGroupBox("Parameters :");
        ui_page4_params_l_ip = new QLabel("Binding address :");
        ui_page4_params_l_port = new QLabel("Listening port :");
        ui_page4_params_colon = new QLabel(":");
        ui_page4_params_ip = new QLineEdit;
        ui_page4_params_ip->setText(settings->value("rtmpgw/ip", "0.0.0.0").toString());
        ui_page4_params_port = new QSpinBox;
        ui_page4_params_port->setRange(1,65335);
        ui_page4_params_port->setValue(settings->value("rtmpgw/port", 80).toInt());
        ui_page4_params_layout = new QGridLayout;
        ui_page4_params_layout->addWidget(ui_page4_params_l_ip, 0, 0);
        ui_page4_params_layout->addWidget(ui_page4_params_l_port, 0, 2);
        ui_page4_params_layout->addWidget(ui_page4_params_ip, 1, 0);
        ui_page4_params_layout->addWidget(ui_page4_params_colon, 1, 1);
        ui_page4_params_layout->addWidget(ui_page4_params_port, 1, 2);
        ui_page4_params_box->setLayout(ui_page4_params_layout);
        ui_page4_verbosity_box = new QGroupBox("Verbosity :");
        ui_page4_verbosity_normal = new QRadioButton("Normal");
        ui_page4_verbosity_normal->setChecked(true);
        ui_page4_verbosity_verbose = new QRadioButton("Verbose");
        ui_page4_verbosity_debug = new QRadioButton("Debug");
        ui_page4_verbosity_layout = new QHBoxLayout;
        ui_page4_verbosity_layout->addWidget(ui_page4_verbosity_normal);
        ui_page4_verbosity_layout->addWidget(ui_page4_verbosity_verbose);
        ui_page4_verbosity_layout->addWidget(ui_page4_verbosity_debug);
        ui_page4_verbosity_box->setLayout(ui_page4_verbosity_layout);
        ui_page4_start = new QPushButton("Start");
        //Layout
        ui_page4_layout = new QVBoxLayout;
        ui_page4_layout->addWidget(ui_page4_params_box);
        ui_page4_layout->addWidget(ui_page4_verbosity_box);
        ui_page4_layout->addWidget(ui_page4_start);
        ui_page4->setLayout(ui_page4_layout);

        //Page 5 : About
        ui_page5 = new QWidget;
        ui_page5_copyrightNotice = new QLabel(QString("<p align=\"center\"><b>Jtv live downloader v.%1</b><br />Copyright © 2012 toine512<br />Compiled on [%2] [%3]<br /><br />This software is distributed under <a href=\"https://www.gnu.org/licenses/gpl.html\">GNU General Public License v. 3</a>.</p><p>Written in C++ with <a href=\"https://qt.nokia.com/\">Qt</a> 4.7.4 (<a href=\"https://www.gnu.org/licenses/gpl.html\">GNU GPL v. 3</a>)<br />Uses <a href=\"http://www.famfamfam.com/lab/icons/silk/\">FAMFAMFAM Silk Icons</a> by <a href=\"http://www.famfamfam.com/\">Mark James</a> (<a href=\"https://creativecommons.org/licenses/by/2.5/\">CC BY 2.5</a>).</p>").arg(G_VERSION, __DATE__, __TIME__));
        ui_page5_copyrightNotice->setOpenExternalLinks(true);
        ui_page5_gplv3 = new QLabel;
        ui_page5_gplv3->setPixmap(QPixmap(":img/gplv3.png"));
        ui_page5_aboutQt = new QPushButton("About Qt");
        //Layout
        ui_page5_layout = new QGridLayout;
        ui_page5_layout->addWidget(ui_page5_copyrightNotice, 0, 0, 1, 2);
        ui_page5_layout->addWidget(ui_page5_gplv3, 1, 0, 1, 1);
        ui_page5_layout->addWidget(ui_page5_aboutQt, 1, 1, 1, 1);
        ui_page5->setLayout(ui_page5_layout);

    //QTabWidget setup
    ui_widget->addTab(ui_page0, "Justin.tv");
    ui_widget->addTab(ui_page3, "Watch");
    ui_widget->addTab(ui_page1, "Params");
    ui_widget->addTab(ui_page2, "rtmpdump");
    ui_widget->addTab(ui_page4, "rtmpgw");
    ui_widget->addTab(ui_page5, "About");

    //Central signals/slots
    connect(ui_page0_searchBtn, SIGNAL(clicked()), this, SLOT(Page0_searchChannel()));
    connect(ui_page0_streamSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(Page0_updateStreamDatas(int)));
    connect(ui_page0_gotoWatch, SIGNAL(clicked()), this, SLOT(Page0_gotoWatchAndStart()));
    connect(ui_page1_rtmp, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_page1_swf, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_page1_swfVfy, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_page1_web, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_page1_usherToken, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_page2_file_btn, SIGNAL(clicked()), this, SLOT(Page2_browseFile()));
    connect(ui_page2_pipe_box, SIGNAL(toggled(bool)), this, SLOT(Page2_toggleFileCheck(bool)));
    connect(ui_page2_file_box, SIGNAL(toggled(bool)), this, SLOT(Page2_togglePipeCheck(bool)));
    connect(ui_page2_pipe, SIGNAL(textEdited(const QString &)), this, SLOT(Page2_savePipe(const QString &)));
    connect(ui_page2_start, SIGNAL(clicked()), this, SLOT(Page2_startRtmpdump()));
    connect(ui_page3_player, SIGNAL(textEdited(const QString &)), this, SLOT(Page3_savePlayerPath(const QString &)));
    connect(ui_page3_watchBtn, SIGNAL(clicked()), this, SLOT(Page3_linkedProcessesStart()));
    connect(ui_page4_params_ip, SIGNAL(textEdited(const QString &)), this, SLOT(Page4_saveIp(const QString &)));
    connect(ui_page4_params_port, SIGNAL(valueChanged(int)), this, SLOT(Page4_savePort(int)));
    connect(ui_page4_start, SIGNAL(clicked()), this, SLOT(Page4_startRtmpgw()));
    connect(ui_page5_aboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
    connect(linkedProcess_rtmpgw, SIGNAL(readyReadStandardOutput()), this, SLOT(Page3_rtmpgwOut()));
    connect(linkedProcess_player, SIGNAL(readyReadStandardOutput()), this, SLOT(Page3_playerOut()));

    setCentralWidget(ui_widget);
}

//Page 0 slots
void JtvLiveUiMain::Page0_searchChannel()
{
    if(ui_page0_channel->text().isEmpty())
    {
        QMessageBox::information(this, "Search live channel", "Empty channel name field");
    }
    else
    {
        Page0_defaultStats();
        Page0_lock();
        ui_page0_streamSelector->setDisabled(true);
        ui_page0_streamSelector->clear();
        ui_page0_gotoWatch->setDisabled(true);
        Page1_defaultParams();
        live_channel->startSearch(ui_page0_channel->text());
    }
}

void JtvLiveUiMain::Page0_onMessageChanged(const QString &message)
{
    ui_page0_parsingInfos->setText(message);
}

void JtvLiveUiMain::Page0_onSearchSuccess(QList<JtvLiveStream> *streams)
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
        ui_page0_streamSelector->addItem(name);
    }
    ui_page0_streamSelector->setCurrentIndex(0); //Will call Page0_updateStreamDatas(int) [slot]
    ui_page0_streamSelector->setEnabled(true);
    ui_page0_gotoWatch->setEnabled(true);
    Page0_unlock();
}

void JtvLiveUiMain::Page0_onSearchError(const QString &error)
{
    QMessageBox::warning(this, "Search live channel", QString("An error occured : %1").arg(error));
    Page0_unlock();
}

void JtvLiveUiMain::Page0_updateStreamDatas(int index)
{
    Page0_defaultStats();
    Page1_defaultParams();
    if(index >= 0) //Avoiding crash when the QComboBox is cleared
    {
        const JtvLiveStream &stream = live_channel->getStreams()->at(index);
        Page0_fillStats(stream);
        Page1_fillParams(stream);
    }
}

void JtvLiveUiMain::Page0_gotoWatchAndStart()
{
    ui_widget->setCurrentIndex(1);
    Page3_linkedProcessesStart();
}

//Page 0 protected
void JtvLiveUiMain::Page0_lock()
{
    ui_page0_channel->setDisabled(true);
    ui_page0_searchBtn->setDisabled(true);
}

void JtvLiveUiMain::Page0_unlock()
{
    ui_page0_channel->setEnabled(true);
    ui_page0_searchBtn->setEnabled(true);
}

void JtvLiveUiMain::Page0_defaultStats()
{
    ui_page0_bitrate->setText("bitrate : n/a");
    ui_page0_viewers->setText("viewers : n/a");
    ui_page0_part->setText("part : n/a");
    ui_page0_id->setText("id : n/a");
    ui_page0_node->setText("node : n/a");
}

void JtvLiveUiMain::Page0_fillStats(const JtvLiveStream &stream)
{
    ui_page0_bitrate->setText(QString("bitrate : ").append(stream.bitrate));
    ui_page0_viewers->setText(QString("viewers : ").append(stream.viewers));
    ui_page0_part->setText(QString("part : ").append(stream.part));
    ui_page0_id->setText(QString("id : ").append(stream.id));
    ui_page0_node->setText(QString("node : ").append(stream.node));
}

//Page 1 slot
void JtvLiveUiMain::Page1_buildCliFriendly()
{
    ui_page1_cliFriendly->setPlainText(QString(getCommandEscaped(collectRtmpParams())).append(" -v"));
}

//Page 1 protected
void JtvLiveUiMain::Page1_defaultParams()
{
    ui_page1_rtmp->clear();
    ui_page1_swf->clear();
    ui_page1_web->clear();
    ui_page1_swfVfy->clear();
    ui_page1_usherToken->clear();
    ui_page1_cliFriendly->clear();
}

void JtvLiveUiMain::Page1_fillParams(const JtvLiveStream &stream)
{
    ui_page1_rtmp->setText(stream.rtmp_url);
    ui_page1_swf->setText(QString(settings->value("justin.tv/player", "http://www-cdn.jtvnw.net/widgets/live_site_player.r48dd120956a62c09d48d3e053fc56cbad3ded23b.swf?userAgent=").toString()).append("&channel=").append(stream.channel_name).append("&referer=").append(settings->value("justin.tv/referer", "http://fr.justin.tv/").toString()).append(stream.channel_name));
    ui_page1_web->setText(QString(settings->value("justin.tv/referer", "http://fr.justin.tv/").toString()).append(stream.channel_name));
    if(stream.server_type == JtvLiveStream::UsherServer)
    {
        ui_page1_usherToken->setText(stream.usher_token);
    }
    else if(stream.server_type == JtvLiveStream::AkamaiServer)
    {
        ui_page1_swfVfy->setText(QString(settings->value("justin.tv/player", "http://www-cdn.jtvnw.net/widgets/live_site_player.r48dd120956a62c09d48d3e053fc56cbad3ded23b.swf?userAgent=").toString()).append("&channel=").append(stream.channel_name).append("&referer=").append(settings->value("justin.tv/referer", "http://fr.justin.tv/").toString()).append(stream.channel_name));
    }
    Page1_buildCliFriendly();
}

//Page 2 slots
void JtvLiveUiMain::Page2_browseFile()
{
    ui_page2_file->setText(QFileDialog::getSaveFileName(this, "Save stream", QString(), "Flash video (*.flv)"));
}

//Called by ui_page2_pipe_box
void JtvLiveUiMain::Page2_toggleFileCheck(bool pipe_ckecked)
{
    if(pipe_ckecked)
    {
        ui_page2_file_box->setChecked(false);
    }
    else
    {
        ui_page2_file_box->setChecked(true);
    }
}

//Called by ui_page2_file_box
void JtvLiveUiMain::Page2_togglePipeCheck(bool file_ckecked)
{
    if(file_ckecked)
    {
        ui_page2_pipe_box->setChecked(false);
    }
    else
    {
        ui_page2_pipe_box->setChecked(true);
    }
}

void JtvLiveUiMain::Page2_savePipe(const QString &text)
{
    settings->setValue("rtmpdump/pipe", text);
}

void JtvLiveUiMain::Page2_startRtmpdump()
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
        if(ui_page2_file_box->isChecked())
        {
            if(ui_page2_file->text().isEmpty())
            {
                QMessageBox::warning(this, "Output file", "No output path provided.");
            }
            else
            {
                if(ui_page2_verbosity_verbose->isChecked())
                {
                    args << "-V";
                }
                else if(ui_page2_verbosity_debug->isChecked())
                {
                    args << "-z";
                }
                args << "-o";
                args << ui_page2_file->text();
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
        else if(ui_page2_pipe_box->isChecked())
        {
            if(ui_page2_pipe->text().isEmpty())
            {
                QMessageBox::warning(this, "Piping", "Right side of the pipe is empty.");
            }
            else
            {
                args << "-q";
#ifdef Q_OS_WIN32
                if(!QProcess::startDetached(QString("cmd.exe /c \"%1 %2 | %3\"").arg(settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), getCommandEscaped(args).replace("\\\"", "\"\"\""), ui_page2_pipe->text()))) //epic Windows crap ! -> replace("\\\"", "\"\"\"")
#else
                if(!QProcess::startDetached(settings->value("terminal/terminal", "xterm").toString(), QStringList() << settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2 | %3").arg(settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), getCommandEscaped(args).replace("&", "\\&"), ui_page2_pipe->text()))) //"&" is meaningful under Linux ;) -> replace("&", "\\&")
#endif
                {
                    QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
                }
            }
        }
    }
}

//Page 3 slots
void JtvLiveUiMain::Page3_savePlayerPath(const QString &path)
{
    settings->setValue("watch/player", path);
}

void JtvLiveUiMain::Page3_linkedProcessesStart()
{
    ui_page0_gotoWatch->setDisabled(true);
    ui_page3_watchBtn->setDisabled(true);
    if(ui_page3_player->text().isEmpty())
    {
        QMessageBox::warning(this, "Player", "No player path/command provided.");
        ui_page0_gotoWatch->setEnabled(true);
        ui_page3_watchBtn->setEnabled(true);
    }
    else
    {
        QStringList args = collectRtmpParams();
        if(args.isEmpty())
        {
            QMessageBox::warning(this, "Parameters", "RTMP parameters are empty.");
            ui_page0_gotoWatch->setEnabled(true);
            ui_page3_watchBtn->setEnabled(true);
        }
        else
        {
            args << "-g";
            args << settings->value("watch/port", "21080").toString();
            args << "-f";
            args << settings->value("flash/version", "WIN 11,1,102,62").toString();
            args << "-v";
            //args << QString("-V");
            connect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
            connect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
            connect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
            connect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
#ifdef Q_OS_WIN32
            linkedProcess_rtmpgw->start(settings->value("watch/rtmpgw", "rtmpgw.exe").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#else
            linkedProcess_rtmpgw->start(settings->value("watch/rtmpgw", "rtmpgw").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#endif
            linkedProcess_player->start(ui_page3_player->text(), QStringList(QString("http://127.0.0.1:").append(settings->value("watch/port", "21080").toString())), QIODevice::ReadOnly | QIODevice::Unbuffered);
        }
    }
}

void JtvLiveUiMain::Page3_linkedProcessesError(const QProcess::ProcessError &error)
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
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
    Page3_linkedProcessesTerminate();
}

void JtvLiveUiMain::Page3_linkedProcessesDisconnectTerminate()
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesDisconnectTerminate()));
    Page3_linkedProcessesTerminate();
}

void JtvLiveUiMain::Page3_rtmpgwOut()
{
    ui_page3_rtmpgwOut->appendPlainText(linkedProcess_rtmpgw->readAll());
}

void JtvLiveUiMain::Page3_playerOut()
{
    ui_page3_playerOut->appendPlainText(linkedProcess_player->readAll());
}

//Page 3 protected
void JtvLiveUiMain::Page3_linkedProcessesTerminate()
{
#ifdef Q_OS_WIN32
    linkedProcess_rtmpgw->kill();
#else
    linkedProcess_rtmpgw->terminate();
#endif
    linkedProcess_player->terminate();
    ui_page0_gotoWatch->setEnabled(true);
    ui_page3_watchBtn->setEnabled(true);
}

//Page 4 : slots
void JtvLiveUiMain::Page4_saveIp(const QString &ip)
{
    settings->setValue("rtmpgw/ip", ip);
}

void JtvLiveUiMain::Page4_savePort(int port)
{
    settings->setValue("rtmpgw/port", port);
}

void JtvLiveUiMain::Page4_startRtmpgw()
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
        if(ui_page4_params_ip->text().isEmpty())
        {
            QMessageBox::warning(this, "Binding IP", "No binding address provided.");
        }
        else
        {
            args << "-D";
            args << ui_page4_params_ip->text();
            args << "-g";
            args << QString("%1").arg(ui_page4_params_port->value());
            if(ui_page4_verbosity_verbose->isChecked())
            {
                args << "-V";
            }
            else if(ui_page4_verbosity_debug->isChecked())
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

QStringList JtvLiveUiMain::collectRtmpParams()
{
    QStringList args;
    if(!ui_page1_rtmp->text().isEmpty())
    {
        args << "-r";
        args << ui_page1_rtmp->text();
    }
    if(!ui_page1_swf->text().isEmpty())
    {
        args << "-s";
        args << ui_page1_swf->text();
    }
    if(!ui_page1_swfVfy->text().isEmpty())
    {
        args << "-W";
        args << ui_page1_swfVfy->text();
    }
    if(!ui_page1_web->text().isEmpty())
    {
        args << "-p";
        args << ui_page1_web->text();
    }
    if(!ui_page1_usherToken->text().isEmpty())
    {
        args << "-j";
        args << ui_page1_usherToken->text();
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
