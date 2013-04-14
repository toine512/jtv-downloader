#include "JtvLiveUiTabBasicRtmpdump.h"

JtvLiveUiTabBasicRtmpdump::JtvLiveUiTabBasicRtmpdump(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_settings = settings;
    p_live_channel = live_channel;

    lne_file = new QLineEdit;
    lab_file = new QLabel("Output file:");
    lab_file->setBuddy(lne_file);
    lab_file->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn_file = new QPushButton;
    btn_file->setIcon(QIcon(":img/explorer.png"));
    btn_file->setToolTip("Browse...");

    layout_file = new QHBoxLayout();
    layout_file->addWidget(lne_file);
    layout_file->addWidget(btn_file);

    btn_start = new QPushButton(QIcon(":img/hdd.png"), "Record");

    lab_tip = new QLabel("Press <kbd>CTRL + C</kbd> in the terminal in order to exit rtmpdump gracefully.<br />Your file might be broken otherwise.");
    lab_tip->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    //Layout
    layout = new QVBoxLayout;
    layout->addWidget(lab_file);
    layout->addLayout(layout_file);
    layout->addWidget(btn_start);
    layout->addWidget(lab_tip);

    setLayout(layout);

    connect(btn_file, SIGNAL(clicked()), this, SLOT(openFileBrowser()));
    connect(btn_start, SIGNAL(clicked()), this, SLOT(startRtmpdump()));
}

void JtvLiveUiTabBasicRtmpdump::openFileBrowser()
{
    lne_file->setText(QFileDialog::getSaveFileName(this, "Save stream", QString(), "Flash video (*.flv)"));
}

void JtvLiveUiTabBasicRtmpdump::startRtmpdump()
{
    if(p_live_channel->isReady())
    {
        QStringList args = p_live_channel->getRtmpParams();
        args << "-f";
        args << p_settings->value("flash/version", "WIN 11,1,102,62").toString();
        if(lne_file->text().isEmpty())
        {
            QMessageBox::warning(this, "Output file", "No output path provided.");
        }
        else
        {
            args << "-o";
            args << lne_file->text();
#ifdef Q_OS_WIN
            if(!QProcess::startDetached(p_settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), args))
#else
            if(!QProcess::startDetached(p_settings->value("terminal/terminal", "xterm").toString(), QStringList() << p_settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2").arg(p_settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), JtvLiveChannel::escape4CLI(args))))
#endif
            {
                QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Parameters", "No stream ready yet.");
    }
}
