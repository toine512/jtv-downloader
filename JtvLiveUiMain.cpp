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



    //Core connections


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
    connect(ui_tab0, SIGNAL(askClearParams()), ui_tab1, SLOT(clearParams()));
    connect(ui_tab0, SIGNAL(gotoWatchAndStart()), this, SLOT(onGotoWatchAndStart()));
    connect(ui_tab3, SIGNAL(askBtn_watchEnable()), ui_tab0, SLOT(btn_watchEnable()));
    connect(ui_tab3, SIGNAL(askBtn_watchDisable()), ui_tab0, SLOT(btn_watchDisable()));

    connect(ui_tab2_file_btn, SIGNAL(clicked()), this, SLOT(Tab2_browseFile()));
    connect(ui_tab2_pipe_box, SIGNAL(toggled(bool)), this, SLOT(Tab2_toggleFileCheck(bool)));
    connect(ui_tab2_file_box, SIGNAL(toggled(bool)), this, SLOT(Tab2_togglePipeCheck(bool)));
    connect(ui_tab2_pipe, SIGNAL(textEdited(const QString &)), this, SLOT(Tab2_savePipe(const QString &)));
    connect(ui_tab2_start, SIGNAL(clicked()), this, SLOT(Tab2_startRtmpdump()));

    connect(ui_tab4_params_ip, SIGNAL(textEdited(const QString &)), this, SLOT(Tab4_saveIp(const QString &)));
    connect(ui_tab4_params_port, SIGNAL(valueChanged(int)), this, SLOT(Tab4_savePort(int)));
    connect(ui_tab4_start, SIGNAL(clicked()), this, SLOT(Tab4_startRtmpgw()));
    connect(ui_tab5_aboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
    connect(updater, SIGNAL(updateAvailable(const QString &, const QString &)), this, SLOT(TabUpdate_show(const QString &, const QString &)));
    connect(updater, SIGNAL(updateNotes(const QString &)), ui_tabUpdate_notes, SLOT(setPlainText(const QString &)));


    setCentralWidget(ui_widget);
}

void JtvLiveUiMain::onGotoWatchAndStart()
{
    ui_widget->setCurrentIndex(ui_widget->indexOf(ui_tab3));
    ui_tab3->linkedProcessesStart();
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
#ifdef Q_OS_WIN
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
#ifdef Q_OS_WIN
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
#ifdef Q_OS_WIN
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
    QString notice = QString("Update %1 is available. <a href=\"%2\">%2</a>").arg(new_version_human, dl_link);
    ui_tabUpdate_notice->setText(notice);
    ui_widget->addTab(ui_tabUpdate, QString("A new update is available! (v. %1)").arg(new_version_human));
    //ui_widget->tabBar()->setTabTextColor(ui_widget->indexOf(ui_tabUpdate), QColor(255, 127, 13));
}

QStringList JtvLiveUiMain::collectRtmpParams()
{
    QStringList args;
    /*if(!ui_tab1_rtmp->text().isEmpty())
    {
        args << "-r";
        args << ui_tab1_rtmp->text();
    }
    if(!lne_swf->text().isEmpty())
    {
        args << "-s";
        args << lne_swf->text();
    }
    if(!lne_swf_vfy->text().isEmpty())
    {
        args << "-W";
        args << lne_swf_vfy->text();
    }
    if(!lne_web->text().isEmpty())
    {
        args << "-p";
        args << lne_web->text();
    }
    if(!lne_usher_token->text().isEmpty())
    {
        args << "-j";
        args << lne_usher_token->text();
    }*/
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
