#include "JtvLiveUiTabJustin_tv.h"

JtvLiveUiTabJustin_tv::JtvLiveUiTabJustin_tv(JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_live_channel = live_channel;

    lne_channel = new QLineEdit;
    lab_channel = new QLabel("Channel name :");
    lab_channel->setBuddy(lne_channel);

    btn_favourite = new QPushButton("F");
    btn_favourite->setToolTip("favourite: TODO");
    btn_favourite->setDisabled(true);

    lne_password = new QLineEdit;
    lab_password = new QLabel("Channel password :");
    lab_password->setBuddy(lne_password);

    btn_search = new QSIRPushButton(":img/zoom.png");

    lab_infos = new QLabel("First, type the channel name in the field above and the password if any.");
    lab_infos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    fra_separator = new QFrame;
    fra_separator->setFrameShape(QFrame::HLine);
    fra_separator->setFrameShadow(QFrame::Sunken);

    cbo_selector = new QComboBox;
    cbo_selector->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    cbo_selector->setDisabled(true);

    btn_watch = new QPushButton;
    btn_watch->setIcon(QIcon(":img/television.png"));
    btn_watch->setToolTip("Watch this stream");
    btn_watch->setDisabled(true);

    lab_bitrate = new QLabel;
    lab_bitrate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    lab_viewers = new QLabel;
    lab_viewers->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    lab_part = new QLabel;
    lab_part->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    lab_id = new QLabel;
    lab_id->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    lab_node = new QLabel;
    lab_node->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    //Init with defaults
    defaultStats();

    //Layouts
    layout_search = new QGridLayout;
    layout_search->addWidget(lab_channel, 0, 0);
    layout_search->addWidget(lne_channel, 0, 1);
    layout_search->addWidget(btn_favourite, 0, 2);
    layout_search->addWidget(lab_password, 1, 0);
    layout_search->addWidget(lne_password, 1, 1, 1, 2);
    layout_search->addWidget(btn_search, 0, 3, 2, 1);

    layout_stream = new QHBoxLayout;
    layout_stream->addWidget(cbo_selector);
    layout_stream->addWidget(btn_watch);

    layout_stats = new QGridLayout;
    layout_stats->addWidget(lab_bitrate, 0, 0);
    layout_stats->addWidget(lab_viewers, 0, 1);
    layout_stats->addWidget(lab_part, 0, 2);
    layout_stats->addWidget(lab_id, 1, 0);
    layout_stats->addWidget(lab_node, 1, 1);

    layout = new QVBoxLayout();
    layout->addLayout(layout_search);
    layout->addWidget(lab_infos);
    layout->addWidget(fra_separator);
    layout->addLayout(layout_stream);
    layout->addLayout(layout_stats);

    setLayout(layout);

    connect(lne_channel, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(lne_password, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(btn_search, SIGNAL(clicked()), this, SLOT(searchChannel()));
    connect(cbo_selector, SIGNAL(currentIndexChanged(int)), p_live_channel, SLOT(setCurrentStream(int)));
    connect(btn_watch, SIGNAL(clicked()), this, SIGNAL(gotoWatchAndStart()));
    connect(p_live_channel, SIGNAL(messageChanged(const QString &)), this, SLOT(onMessageChanged(const QString &)));
    connect(p_live_channel, SIGNAL(channelSearchSuccess(QStringList)), this, SLOT(onSearchSuccess(QStringList)));
    connect(p_live_channel, SIGNAL(channelSearchError(const QString &)), this, SLOT(onSearchError(const QString &)));
    connect(p_live_channel, SIGNAL(streamChanged()), this, SLOT(fillStats()));
}

void JtvLiveUiTabJustin_tv::btn_watchEnable()
{
    btn_watch->setEnabled(true);
}

void JtvLiveUiTabJustin_tv::btn_watchDisable()
{
    btn_watch->setDisabled(true);
}

void JtvLiveUiTabJustin_tv::searchChannel()
{
    if(lne_channel->text().isEmpty())
    {
        QMessageBox::information(this, "Search live channel", "Empty channel name field");
    }
    else
    {
        defaultStats();
        lock();
        cbo_selector->setDisabled(true);
        cbo_selector->clear();
        btn_watch->setDisabled(true);
        emit askClearParams();
        p_live_channel->startSearch(lne_channel->text(), lne_password->text());
    }
}

void JtvLiveUiTabJustin_tv::onMessageChanged(const QString &message)
{
    lab_infos->setText(message);
}

void JtvLiveUiTabJustin_tv::onSearchSuccess(QStringList names)
{
    foreach(const QString &name, names)
    {
        cbo_selector->addItem(name);
    }
    cbo_selector->setCurrentIndex(0); //Will call Tab0_updateStreamDatas(int) [slot]
    cbo_selector->setEnabled(true);
    btn_watch->setEnabled(true);
    unlock();
}

void JtvLiveUiTabJustin_tv::onSearchError(const QString &error)
{
    QMessageBox::warning(this, "Search live channel", QString("An error occured : %1").arg(error));
    unlock();
}

void JtvLiveUiTabJustin_tv::lock()
{
    lne_channel->setDisabled(true);
    btn_search->setDisabled(true);
    lne_password->setDisabled(true);
}

void JtvLiveUiTabJustin_tv::unlock()
{
    lne_channel->setEnabled(true);
    btn_search->setEnabled(true);
    lne_password->setEnabled(true);
}

void JtvLiveUiTabJustin_tv::defaultStats()
{
    lab_bitrate->setText("bitrate : n/a");
    lab_viewers->setText("viewers : n/a");
    lab_part->setText("part : n/a");
    lab_id->setText("id : n/a");
    lab_node->setText("node : n/a");
}

void JtvLiveUiTabJustin_tv::fillStats()
{
    lab_bitrate->setText(QString("bitrate : ").append(p_live_channel->getStreamBitrate()));
    lab_viewers->setText(QString("viewers : ").append(p_live_channel->getStreamViewers()));
    lab_part->setText(QString("part : ").append(p_live_channel->getStreamPart()));
    lab_id->setText(QString("id : ").append(p_live_channel->getStreamId()));
    lab_node->setText(QString("node : ").append(p_live_channel->getStreamNode()));
}
