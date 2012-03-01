#include "JtvLiveUiMain.h"
#include <QDesktopWidget>
#include <QtCore/Qt>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

JtvLiveUiMain::JtvLiveUiMain(QWidget *parent) :
    QMainWindow(parent)
{
    //Core
    settings = new QSettings("jtvdl.conf", QSettings::IniFormat, this);
    live_channel = new JtvLiveChannel;
    connect(live_channel, SIGNAL(messageChanged(const QString &)), this, SLOT(Page0_onMessageChanged(const QString &)));
    connect(live_channel, SIGNAL(channelSearchSuccess(QList<JtvLiveStream> *)), this, SLOT(Page0_onSearchSuccess(QList<JtvLiveStream> *)));
    connect(live_channel, SIGNAL(channelSearchError(const QString &)), this, SLOT(Page0_onSearchError(const QString &)));
    linkedProcess_rtmpgw = new QProcess(this);
    linkedProcess_rtmpgw->setProcessChannelMode(QProcess::MergedChannels);
    linkedProcess_player = new QProcess(this);
    linkedProcess_player->setProcessChannelMode(QProcess::MergedChannels);

    setWindowTitle("Justin.tv live downloader");
    //resize(425, 240);
    setFixedSize(496, 280);
    //Center on the current screen
    QDesktopWidget desktop_widget;
    QRect screen_geometry = desktop_widget.availableGeometry();
    move(screen_geometry.width() / 2 - width() / 2, screen_geometry.height() / 2 - height() / 2);

    //Statusbar
    ui_bottom_statusBar = statusBar();
    ui_bottom_statusBar->showMessage("I'm an useless status bar !");

    //Central zone
    ui_central_widget = new QTabWidget;

        //Page 0 : Justin.tv
        ui_central_page0 = new QWidget;
        ui_central_page0_chanName = new QLabel("Channel name :");
        ui_central_page0_chanName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_central_page0_channel = new QLineEdit;
        ui_central_page0_searchBtn = new QPushButton;
        ui_central_page0_searchBtn->setIcon(QIcon(":img/zoom.png"));
        ui_central_page0_parsingInfos = new QLabel("First, write the channel name in the field above.");
        ui_central_page0_parsingInfos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_central_page0_hSeparator = new QFrame;
        ui_central_page0_hSeparator->setFrameShape(QFrame::HLine);
        ui_central_page0_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_central_page0_streamSelector = new QComboBox;
        ui_central_page0_streamSelector->setEnabled(false);
        ui_central_page0_bitrate = new QLabel();
        ui_central_page0_bitrate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_viewers = new QLabel();
        ui_central_page0_viewers->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_part = new QLabel();
        ui_central_page0_part->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_id = new QLabel();
        ui_central_page0_id->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_node = new QLabel();
        ui_central_page0_node->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        Page0_defaultStats();
        //Layouts
        ui_central_page0_searchLayout = new QHBoxLayout;
        ui_central_page0_searchLayout->addWidget(ui_central_page0_chanName);
        ui_central_page0_searchLayout->addWidget(ui_central_page0_channel);
        ui_central_page0_searchLayout->addWidget(ui_central_page0_searchBtn);
        ui_central_page0_layout = new QGridLayout;
        ui_central_page0_layout->addLayout(ui_central_page0_searchLayout, 0, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_parsingInfos, 1, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_hSeparator, 2, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_streamSelector, 3, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_bitrate, 4, 0);
        ui_central_page0_layout->addWidget(ui_central_page0_viewers, 4, 1);
        ui_central_page0_layout->addWidget(ui_central_page0_part, 4, 2);
        ui_central_page0_layout->addWidget(ui_central_page0_id, 5, 0);
        ui_central_page0_layout->addWidget(ui_central_page0_node, 5, 1);
        ui_central_page0->setLayout(ui_central_page0_layout);

        //Page 1 : Parameters
        ui_central_page1 = new QWidget;
        ui_central_page1_l_rtmp = new QLabel("RTMP (-r)");
        ui_central_page1_rtmp = new QLineEdit;
        ui_central_page1_l_swf = new QLabel("SWF (-s)");
        ui_central_page1_swf = new QLineEdit;
        ui_central_page1_l_swfVfy = new QLabel("SWF verif (-W)");
        ui_central_page1_swfVfy = new QLineEdit;
        ui_central_page1_l_web = new QLabel("Web page (-p)");
        ui_central_page1_web = new QLineEdit;
        ui_central_page1_l_usherToken = new QLabel("UsherToken (-j)");
        ui_central_page1_usherToken = new QLineEdit;
        ui_central_page1_hSeparator = new QFrame;
        ui_central_page1_hSeparator->setFrameShape(QFrame::HLine);
        ui_central_page1_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_central_page1_l_cliFriendly = new QLabel("Command line input firendly :");
        ui_central_page1_cliFriendly = new QPlainTextEdit;
        ui_central_page1_cliFriendly->setReadOnly(true);
        //Layout
        ui_central_page1_layout = new QGridLayout;
        ui_central_page1_layout->addWidget(ui_central_page1_l_rtmp, 0, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_rtmp, 0, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_swf, 1, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_swf, 1, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_swfVfy, 2, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_swfVfy, 2, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_web, 3, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_web, 3, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_usherToken, 4, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_usherToken, 4, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_hSeparator, 5, 0, 1, 2);
        ui_central_page1_layout->addWidget(ui_central_page1_l_cliFriendly, 6, 0, 1, 2);
        ui_central_page1_layout->addWidget(ui_central_page1_cliFriendly, 7, 0, 1, 2);
        ui_central_page1->setLayout(ui_central_page1_layout);

        //Page 2 : rtmpdump
        ui_central_page2 = new QWidget;
        ui_central_page2_file_box = new QGroupBox("Outfile :");
        ui_central_page2_file_box->setCheckable(true);
        ui_central_page2_file_box->setChecked(true);
        ui_central_page2_file = new QLineEdit;
        ui_central_page2_file_btn = new QPushButton("...");
        ui_central_page2_file_layout = new QHBoxLayout();
        ui_central_page2_file_layout->addWidget(ui_central_page2_file);
        ui_central_page2_file_layout->addWidget(ui_central_page2_file_btn);
        ui_central_page2_file_box->setLayout(ui_central_page2_file_layout);
        ui_central_page2_pipe_box = new QGroupBox("Pipe :");
        ui_central_page2_pipe_box->setCheckable(true);
        ui_central_page2_pipe_box->setChecked(false);
        ui_central_page2_pipe = new QLineEdit;
        ui_central_page2_pipe_layout = new QHBoxLayout;
        ui_central_page2_pipe_layout->addWidget(ui_central_page2_pipe);
        ui_central_page2_pipe_box->setLayout(ui_central_page2_pipe_layout);
        ui_central_page2_verbosity_box = new QGroupBox("Verbosity :");
        ui_central_page2_verbosity_normal = new QRadioButton("Normal");
        ui_central_page2_verbosity_normal->setChecked(true);
        ui_central_page2_verbosity_verbose = new QRadioButton("Verbose");
        ui_central_page2_verbosity_debug = new QRadioButton("Debug");
        ui_central_page2_verbosity_layout = new QHBoxLayout;
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_normal);
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_verbose);
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_debug);
        ui_central_page2_verbosity_box->setLayout(ui_central_page2_verbosity_layout);
        ui_central_page2_start = new QPushButton("Start");
        //Layout
        ui_central_page2_layout = new QVBoxLayout;
        ui_central_page2_layout->addWidget(ui_central_page2_file_box);
        ui_central_page2_layout->addWidget(ui_central_page2_pipe_box);
        ui_central_page2_layout->addWidget(ui_central_page2_verbosity_box);
        ui_central_page2_layout->addWidget(ui_central_page2_start);
        ui_central_page2->setLayout(ui_central_page2_layout);

        //Page 3 : Play
        ui_central_page3 = new QWidget;
        ui_central_page3_player = new QLineEdit;
#ifdef Q_OS_WIN32
        ui_central_page3_player_label = new QLabel("Player path :");
        ui_central_page3_player->setText(settings->value("watch/player", "%programfiles%\\VideoLAN\\VLC\\vlc.exe").toString());
#else
        ui_central_page3_player_label = new QLabel("Player command :");
        ui_central_page3_player->setText(settings->value("watch/player", "vlc").toString());
#endif
        ui_central_page3_player_layout = new QHBoxLayout;
        ui_central_page3_player_layout->addWidget(ui_central_page3_player_label);
        ui_central_page3_player_layout->addWidget(ui_central_page3_player);
        ui_central_page3_watchBtn = new QPushButton("Watch");
        ui_central_page3_hSeparator = new QFrame;
        ui_central_page3_hSeparator->setFrameShape(QFrame::HLine);
        ui_central_page3_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_central_page3_rtmpgwOut = new QPlainTextEdit;
        ui_central_page3_rtmpgwOut->setReadOnly(true);
        ui_central_page3_playerOut = new QPlainTextEdit;
        ui_central_page3_playerOut->setReadOnly(true);
        ui_central_page3_layout = new QVBoxLayout;
        ui_central_page3_layout->addLayout(ui_central_page3_player_layout);
        ui_central_page3_layout->addWidget(ui_central_page3_watchBtn);
        ui_central_page3_layout->addWidget(ui_central_page3_hSeparator);
        ui_central_page3_layout->addWidget(ui_central_page3_rtmpgwOut);
        ui_central_page3_layout->addWidget(ui_central_page3_playerOut);
        ui_central_page3->setLayout(ui_central_page3_layout);

        //Page 4 : Control
        ui_central_page4 = new QWidget;

    //QTabWidget setup
    ui_central_widget->addTab(ui_central_page0, "Justin.tv");
    ui_central_widget->addTab(ui_central_page3, "Play");
    ui_central_widget->addTab(ui_central_page1, "Params");
    ui_central_widget->addTab(ui_central_page2, "rtmpdump");
    ui_central_widget->addTab(ui_central_page4, "rtmpgw");

    //Central signals/slots
    connect(ui_central_page0_searchBtn, SIGNAL(clicked()), this, SLOT(Page0_searchChannel()));
    connect(ui_central_page0_streamSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(Page0_updateStreamDatas(int)));
    connect(ui_central_page1_rtmp, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_central_page1_swf, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_central_page1_swfVfy, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_central_page1_web, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_central_page1_usherToken, SIGNAL(textEdited(QString)), this, SLOT(Page1_buildCliFriendly()));
    connect(ui_central_page2_file_btn, SIGNAL(clicked()), this, SLOT(Page2_browseFile()));
    connect(ui_central_page2_pipe_box, SIGNAL(toggled(bool)), this, SLOT(Page2_toggleFileCheck(bool)));
    connect(ui_central_page2_file_box, SIGNAL(toggled(bool)), this, SLOT(Page2_togglePipeCheck(bool)));
    connect(ui_central_page3_player, SIGNAL(textEdited(const QString &)), this, SLOT(Page3_savePlayerPath(const QString &)));
    connect(ui_central_page3_watchBtn, SIGNAL(clicked()), this, SLOT(Page3_linkedProcessesStart()));
    connect(linkedProcess_rtmpgw, SIGNAL(readyReadStandardOutput()), this, SLOT(Page3_rtmpgwOut()));
    connect(linkedProcess_player, SIGNAL(readyReadStandardOutput()), this, SLOT(Page3_playerOut()));

    setCentralWidget(ui_central_widget);
}

QStringList JtvLiveUiMain::collectRtmpParams()
{
    QStringList args;
    if(!ui_central_page1_rtmp->text().isEmpty())
    {
        args << "-r";
        args << ui_central_page1_rtmp->text();
    }
    if(!ui_central_page1_swf->text().isEmpty())
    {
        args << "-s";
        args << ui_central_page1_swf->text();
    }
    if(!ui_central_page1_swfVfy->text().isEmpty())
    {
        args << "-W";
        args << ui_central_page1_swfVfy->text();
    }
    if(!ui_central_page1_web->text().isEmpty())
    {
        args << "-p";
        args << ui_central_page1_web->text();
    }
    if(!ui_central_page1_usherToken->text().isEmpty())
    {
        args << "-j";
        args << ui_central_page1_usherToken->text();
    }
    return args;
}

void JtvLiveUiMain::Page0_lock()
{
    ui_central_page0_channel->setDisabled(true);
    ui_central_page0_searchBtn->setDisabled(true);
}

void JtvLiveUiMain::Page0_unlock()
{
    ui_central_page0_channel->setEnabled(true);
    ui_central_page0_searchBtn->setEnabled(true);
}

void JtvLiveUiMain::Page0_defaultStats()
{
    ui_central_page0_bitrate->setText("bitrate : n/a");
    ui_central_page0_viewers->setText("viewers : n/a");
    ui_central_page0_part->setText("part : n/a");
    ui_central_page0_id->setText("id : n/a");
    ui_central_page0_node->setText("node : n/a");
}

void JtvLiveUiMain::Page0_fillStats(const JtvLiveStream &stream)
{
    ui_central_page0_bitrate->setText(QString("bitrate : ").append(stream.bitrate));
    ui_central_page0_viewers->setText(QString("viewers : ").append(stream.viewers));
    ui_central_page0_part->setText(QString("part : ").append(stream.part));
    ui_central_page0_id->setText(QString("id : ").append(stream.id));
    ui_central_page0_node->setText(QString("node : ").append(stream.node));
}

void JtvLiveUiMain::Page1_defaultParams()
{
    ui_central_page1_rtmp->clear();
    ui_central_page1_swf->clear();
    ui_central_page1_web->clear();
    ui_central_page1_usherToken->clear();
    ui_central_page1_swfVfy->clear();
    ui_central_page1_cliFriendly->clear();
}

void JtvLiveUiMain::Page1_fillParams(const JtvLiveStream &stream)
{
    ui_central_page1_rtmp->setText(stream.rtmp_url);
    ui_central_page1_swf->setText(QString(stream.player_url).append("?channel=").append(stream.channel_name));
    ui_central_page1_web->setText(QString("http://fr.justin.tv/").append(stream.channel_name));
    if(stream.server_type == LEGACY)
    {
        ui_central_page1_usherToken->setText(stream.usher_token);
    }
    else if(stream.server_type == AKAMAI)
    {
        ui_central_page1_swfVfy->setText(QString(stream.player_url).append("?channel=").append(stream.channel_name));
    }
    Page1_buildCliFriendly();
}

void JtvLiveUiMain::Page1_buildCliFriendly()
{
    QString cmd = QString("-r %1 -s %2 -p %3 -v").arg(ui_central_page1_rtmp->text(), ui_central_page1_swf->text(), ui_central_page1_web->text());
    if(!ui_central_page1_swfVfy->text().isEmpty())
    {
        cmd.append(QString(" -W ").append(ui_central_page1_swfVfy->text()));
    }
    if(!ui_central_page1_usherToken->text().isEmpty())
    {
        cmd.append(QString(" -j \"%1\"").arg(ui_central_page1_usherToken->text().replace("\"", "\\\"")));
    }
    ui_central_page1_cliFriendly->setPlainText(cmd);
}

void JtvLiveUiMain::Page0_searchChannel()
{
    QString channel_name = ui_central_page0_channel->text();
    if(channel_name.isEmpty())
    {
        QMessageBox::information(this, "Search live channel", "Empty channel name field");
    }
    else
    {
        Page0_defaultStats();
        Page0_lock();
        Page1_defaultParams();
        ui_central_page0_streamSelector->setDisabled(true);
        ui_central_page0_streamSelector->clear();
        live_channel->startSearch(channel_name);
    }
}

void JtvLiveUiMain::Page0_onMessageChanged(const QString &message)
{
    ui_central_page0_parsingInfos->setText(message);
}

void JtvLiveUiMain::Page0_onSearchSuccess(QList<JtvLiveStream> *streams)
{
    int s = streams->size();
    for(int i = 0 ; i < s ; i++)
    {
        QString name = QString("%1\t\tHeight : %2\t\t").arg(streams->at(i).tag_name, streams->at(i).height);
        if(streams->at(i).server_type == LEGACY)
        {
            name.append("UsherToken");
        }
        else if(streams->at(i).server_type == AKAMAI)
        {
            name.append("SWF Vfy");
        }
        ui_central_page0_streamSelector->addItem(name);
    }
    ui_central_page0_streamSelector->setCurrentIndex(0); //Will call Page0_updateStreamDatas(int) [slot]
    ui_central_page0_streamSelector->setEnabled(true);
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

void JtvLiveUiMain::Page2_browseFile()
{
    ui_central_page2_file->setText(QFileDialog::getSaveFileName(this, "Save stream", QString(), "Flash video (*.flv)"));
}

//Called by ui_central_page2_pipe_box
void JtvLiveUiMain::Page2_toggleFileCheck(bool pipe_ckecked)
{
    if(pipe_ckecked)
    {
        ui_central_page2_file_box->setChecked(false);
    }
    else
    {
        ui_central_page2_file_box->setChecked(true);
    }
}

//Called by ui_central_page2_file_box
void JtvLiveUiMain::Page2_togglePipeCheck(bool file_ckecked)
{
    if(file_ckecked)
    {
        ui_central_page2_pipe_box->setChecked(false);
    }
    else
    {
        ui_central_page2_pipe_box->setChecked(true);
    }
}

void JtvLiveUiMain::Page3_savePlayerPath(const QString &path)
{
    settings->setValue("watch/player", path);
}

void JtvLiveUiMain::Page3_linkedProcessesStart()
{
    ui_central_page3_watchBtn->setDisabled(true);
    if(ui_central_page3_player->text().isEmpty())
    {
        QMessageBox::warning(this, "Player", "No player path/command provided.");
        ui_central_page3_watchBtn->setEnabled(true);
    }
    else
    {
        QStringList args = collectRtmpParams();
        if(args.isEmpty())
        {
            QMessageBox::warning(this, "Parameters", "RTMP parameters are empty.");
            ui_central_page3_watchBtn->setEnabled(true);
        }
        else
        {
            args << QString("-g ").append(settings->value("watch/port", "21080").toString());
            args << QString("-f ").append(settings->value("flash/version", "WIN 11,1,102,62").toString());
            args << QString("-v");
            //args << QString("-V");
            connect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
            connect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
            connect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
            connect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
#ifdef Q_OS_WIN32
            linkedProcess_rtmpgw->start(settings->value("rtmp/rtmpgw", "rtmpgw.exe").toString(), args, QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
#else
            linkedProcess_rtmpgw->start(settings->value("rtmp/rtmpgw", "rtmpgw").toString(), args, QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
#endif
            linkedProcess_player->start(ui_central_page3_player->text(), QStringList(QString("http://127.0.0.1:").append(settings->value("watch/port", "21080").toString())));
        }
    }
}

void JtvLiveUiMain::Page3_linkedProcessesError(const QProcess::ProcessError &error)
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
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

void JtvLiveUiMain::Page3_linkedProcessesTerminate()
{
    disconnect(linkedProcess_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
    disconnect(linkedProcess_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(Page3_linkedProcessesError(QProcess::ProcessError)));
    disconnect(linkedProcess_rtmpgw, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
    disconnect(linkedProcess_player, SIGNAL(finished(int)), this, SLOT(Page3_linkedProcessesTerminate()));
#ifdef Q_OS_WIN32
    linkedProcess_rtmpgw->kill();
#else
    linkedProcess_rtmpgw->terminate();
#endif
    linkedProcess_player->terminate();
    ui_central_page3_watchBtn->setEnabled(true);
}

void JtvLiveUiMain::Page3_rtmpgwOut()
{
    ui_central_page3_rtmpgwOut->appendPlainText(linkedProcess_rtmpgw->readAll());
}

void JtvLiveUiMain::Page3_playerOut()
{
    ui_central_page3_playerOut->appendPlainText(linkedProcess_player->readAll());
}

JtvLiveUiMain::~JtvLiveUiMain()
{ }
