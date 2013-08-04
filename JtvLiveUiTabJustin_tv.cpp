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

#include "JtvLiveUiTabJustin_tv.h"



JtvLiveUiTabJustin_tv::JtvLiveUiTabJustin_tv(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_live_channel = live_channel;

    p_favourites = new FavCompleter(settings, "justin.tv/favourites", this);

    lne_channel = new QLineEdit;
    lne_channel->setCompleter(p_favourites);
    lab_channel = new QLabel("Channel name :");
    lab_channel->setBuddy(lne_channel);

    btn_favourite = new QPushButton;
    btn_favouriteSetState(false);

    lne_password = new QLineEdit;
    lne_password->setEchoMode(QLineEdit::Password);
    lab_password = new QLabel("Channel password :");
    lab_password->setBuddy(lne_password);

    btn_search = new QSquareIconResizingPushButton(":img/search.png");

    lab_infos = new QLabel("First, type the channel name in the field above and the password if any.");
    lab_infos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    fra_separator = new QFrame;
    fra_separator->setFrameShape(QFrame::HLine);
    fra_separator->setFrameShadow(QFrame::Sunken);
    cbo_geo = new QComboBox;
    cbo_geo->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    cbo_geo->setDisabled(false);


    cbo_geo->addItem("STANDARD CHANNEL");
    cbo_geo->addItem("US WEST");
    cbo_geo->addItem("US CENTER");
    cbo_geo->addItem("US EAST");
    cbo_geo->addItem("EU 1");
    cbo_geo->addItem("EU 2");
    cbo_geo->addItem("EAST ASIA");

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

    btn_simplified = new QPushButton("Simplified GUI");

    //Init with defaults
    defaultStats();

    //Layouts
    layout_search = new QGridLayout;
    layout_search->addWidget(lab_channel, 0, 0);
    layout_search->addWidget(lne_channel, 0, 1);
    layout_search->addWidget(btn_favourite, 1, 2);
    layout_search->addWidget(lab_password, 1, 0);
    layout_search->addWidget(lne_password, 1, 1);
    layout_search->addWidget(btn_search, 0, 4, 2, 1);
    layout_search->addWidget(cbo_geo, 0, 2);

    layout_stream = new QHBoxLayout;
    layout_stream->addWidget(cbo_selector);
    layout_stream->addWidget(btn_watch);

    layout_stats = new QGridLayout;
    layout_stats->addWidget(lab_bitrate, 0, 0);
    layout_stats->addWidget(lab_viewers, 0, 1);
    layout_stats->addWidget(lab_part, 0, 2);
    layout_stats->addWidget(lab_id, 1, 0);
    layout_stats->addWidget(lab_node, 1, 1);
    layout_stats->addWidget(btn_simplified, 1, 2);

    layout = new QVBoxLayout();
    layout->addLayout(layout_search);
    layout->addWidget(lab_infos);
    layout->addWidget(fra_separator);
    layout->addLayout(layout_stream);
    layout->addLayout(layout_stats);

    setLayout(layout);

    connect(lne_channel, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(lne_channel, SIGNAL(textChanged(const QString &)), this, SLOT(verifyChannelFavouriting(const QString &)));
    connect(lne_password, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(btn_favourite, SIGNAL(clicked()), this, SLOT(toggleChannelFavourite()));
    connect(btn_search, SIGNAL(clicked()), this, SLOT(searchChannel()));



    connect(cbo_selector, SIGNAL(currentIndexChanged(int)), p_live_channel, SLOT(setCurrentStream(int)));
    connect(btn_watch, SIGNAL(clicked()), this, SIGNAL(gotoWatchAndStart()));
    connect(btn_simplified, SIGNAL(clicked()), this, SIGNAL(toggleUi()));
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

void JtvLiveUiTabJustin_tv::toggleChannelFavourite()
{
    QString ch = lne_channel->text();
    if(!ch.isEmpty())
    {
        btn_favouriteSetState(p_favourites->toggleFavourite(ch));
    }
}

void JtvLiveUiTabJustin_tv::verifyChannelFavouriting(const QString &s)
{
    if(b_btn_favourite_isFav != p_favourites->isFavourited(s))
    {
        btn_favouriteSetState(!b_btn_favourite_isFav);
    }
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
    btn_favourite->setDisabled(true);
    btn_search->setDisabled(true);
    lne_password->setDisabled(true);
}

void JtvLiveUiTabJustin_tv::unlock()
{
    lne_channel->setEnabled(true);
    btn_favourite->setEnabled(true);
    btn_search->setEnabled(true);
    lne_password->setEnabled(true);
}

void JtvLiveUiTabJustin_tv::btn_favouriteSetState(bool isFav)
{
    b_btn_favourite_isFav = isFav;
    if(b_btn_favourite_isFav)
    {
        btn_favourite->setIcon(QIcon(":img/delete.png"));
        btn_favourite->setToolTip("Remove bookmark");
    }
    else
    {
        btn_favourite->setIcon(QIcon(":img/heart.png"));
        btn_favourite->setToolTip("Add bookmark");
    }
}

void JtvLiveUiTabJustin_tv::defaultStats()
{
    lab_bitrate->setText("bitrate : n/a");
    lab_viewers->setText("viewers : n/a");
    lab_part->setText("part : n/a");
    lab_id->setText("id : n/a");
    lab_node->setText("node : n/a");
    QString choix;
    choix=cbo_geo->currentText();
    if (choix=="STANDARD CHANNEL")      valeur="http://usher.justin.tv/find/%1.xml?type=any&";
    else if (choix=="US WEST")   valeur="http://2.my-proxy.com/browse.php";
    else if (choix=="US CENTER")   valeur="http://1.my-proxy.com/browse.php";
    else if (choix=="US EAST")   valeur="http://betterproxy.org/browse.php";
    else if (choix=="EU 1")   valeur="http://cartogis.ugent.be/kooms/proxy/proxy_iframe/index.php?q=http://usher.justin.tv/find/%1.xml?type=any&";
    else if (choix=="EU 2")   valeur="http://rootproxy.com/browse.php";
    else if (choix=="EAST ASIA") valeur="http://anzas21.sakura.ne.jp/proxy/browse.php";
    else valeur="http://usher.justin.tv/find/%1.xml?type=any&";





}

void JtvLiveUiTabJustin_tv::fillStats()
{

    lab_bitrate->setText(QString("bitrate : ").append(p_live_channel->getStreamBitrate()));
    lab_viewers->setText(QString("viewers : ").append(p_live_channel->getStreamViewers()));
    lab_part->setText(QString("part : ").append(p_live_channel->getStreamPart()));
    lab_id->setText(QString("id : ").append(p_live_channel->getStreamId()));
    lab_node->setText(QString("node : ").append(p_live_channel->getStreamNode()));

}
