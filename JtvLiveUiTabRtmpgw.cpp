#include "JtvLiveUiTabRtmpgw.h"

JtvLiveUiTabRtmpgw::JtvLiveUiTabRtmpgw(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_settings = settings;
    p_live_channel = live_channel;

    group_parameters = new QGroupBox("Parameters :");

    lab_ip = new QLabel("Binding address :");

    lab_port = new QLabel("Listening port :");

    lab_colon = new QLabel(":");

    lne_ip = new QLineEdit;
    lne_ip->setText(p_settings->value("rtmpgw/ip", "0.0.0.0").toString());

    spb_port = new QSpinBox;
    spb_port->setRange(1,65335);
    spb_port->setValue(p_settings->value("rtmpgw/port", 80).toInt());

    layout_parameters = new QGridLayout;
    layout_parameters->addWidget(lab_ip, 0, 0);
    layout_parameters->addWidget(lab_port, 0, 2);
    layout_parameters->addWidget(lne_ip, 1, 0);
    layout_parameters->addWidget(lab_colon, 1, 1);
    layout_parameters->addWidget(spb_port, 1, 2);

    group_parameters->setLayout(layout_parameters);

    group_verbosity = new QGroupBox("Verbosity :");

    rbtn_verbosity_normal = new QRadioButton("Normal");
    rbtn_verbosity_normal->setChecked(true);

    rbtn_verbosity_verbose = new QRadioButton("Verbose");

    rbtn_verbosity_debug = new QRadioButton("Debug");

    layout_verbosity = new QHBoxLayout;
    layout_verbosity->addWidget(rbtn_verbosity_normal);
    layout_verbosity->addWidget(rbtn_verbosity_verbose);
    layout_verbosity->addWidget(rbtn_verbosity_debug);

    group_verbosity->setLayout(layout_verbosity);

    btn_start = new QPushButton("Start");

    //Layout
    layout = new QVBoxLayout;
    layout->addWidget(group_parameters);
    layout->addWidget(group_verbosity);
    layout->addWidget(btn_start);

    setLayout(layout);

    connect(lne_ip, SIGNAL(textEdited(const QString &)), this, SLOT(settingsSetIp(const QString &)));
    connect(spb_port, SIGNAL(valueChanged(int)), this, SLOT(settingsSetPort(int)));
    connect(btn_start, SIGNAL(clicked()), this, SLOT(startRtmpgw()));
}

void JtvLiveUiTabRtmpgw::settingsSetIp(const QString &ip)
{
    p_settings->setValue("rtmpgw/ip", ip);
}

void JtvLiveUiTabRtmpgw::settingsSetPort(int port)
{
    p_settings->setValue("rtmpgw/port", port);
}

void JtvLiveUiTabRtmpgw::startRtmpgw()
{
    if(p_live_channel->isReady())
    {
        QStringList args = p_live_channel->getRtmpParams();
        args << "-f";
        args << p_settings->value("flash/version", "WIN 11,1,102,62").toString();
        if(lne_ip->text().isEmpty())
        {
            QMessageBox::warning(this, "Binding IP", "No binding address provided.");
        }
        else
        {
            args << "-D";
            args << lne_ip->text();
            args << "-g";
            args << QString("%1").arg(spb_port->value());
            if(rbtn_verbosity_verbose->isChecked())
            {
                args << "-V";
            }
            else if(rbtn_verbosity_debug->isChecked())
            {
                args << "-z";
            }
#ifdef Q_OS_WIN
            if(!QProcess::startDetached(p_settings->value("rtmpgw/rtmpgw", "rtmpgw.exe").toString(), args))
#else
            if(!QProcess::startDetached(p_settings->value("terminal/terminal", "xterm").toString(), QStringList() << p_settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2").arg(p_settings->value("rtmpgw/rtmpgw", "rtmpgw").toString(), JtvLiveChannel::escape4CLI(args))))
#endif
            {
                QMessageBox::warning(this, "Launching rtmpgw", "Unable to create the process, check the path.");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Parameters", "No stream ready yet.");
    }
}
