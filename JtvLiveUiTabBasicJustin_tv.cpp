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

#include "JtvLiveUiTabBasicJustin_tv.h"

JtvLiveUiTabBasicJustin_tv::JtvLiveUiTabBasicJustin_tv(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_live_channel = live_channel;

    p_favourites = new FavCompleter(settings, "justin.tv/favourites", this);

    lne_channel = new QLineEdit;
    lne_channel->setCompleter(p_favourites);
    lab_channel = new QLabel("Channel name :");
    lab_channel->setBuddy(lne_channel);

    /*btn_favourite = new QPushButton("F");
    btn_favourite->setToolTip("favourite: TODO");
    btn_favourite->setDisabled(true);*/
    btn_favourite = new QPushButton;
    btn_favouriteSetState(false);

    lne_password = new QLineEdit;
    lab_password = new QLabel("Channel password :");
    lab_password->setBuddy(lne_password);

    btn_search = new QSquareIconResizingPushButton(":img/search.png");

    /*lab_infos = new QLabel("First, type the channel name in the field above and the password if any.");
    lab_infos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);*/

    fra_separator = new QFrame;
    fra_separator->setFrameShape(QFrame::HLine);
    fra_separator->setFrameShadow(QFrame::Sunken);

    lab_stream = new QLabel("Select a stream:");

    cbo_selector = new QComboBox;
    cbo_selector->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    cbo_selector->setDisabled(true);

    btn_record = new QPushButton("Record");
    btn_record->setIcon(QIcon(":img/hdd32.png"));
    btn_record->setIconSize(QSize(32, 32));
    btn_record->setToolTip("Record this stream");
    btn_record->setDisabled(true);

    btn_watch = new QPushButton("Watch");
    btn_watch->setIcon(QIcon(":img/television32.png"));
    btn_watch->setIconSize(QSize(32, 32));
    btn_watch->setToolTip("Watch this stream");
    btn_watch->setDisabled(true);

    btn_advanced = new QPushButton("Use advanced interface");

    //Layouts
    layout_actions = new QHBoxLayout;
    layout_actions->addWidget(btn_record);
    layout_actions->addWidget(btn_watch);

    layout = new QGridLayout;
    layout->addWidget(lab_channel, 0, 0);
    layout->addWidget(lne_channel, 0, 1);
    layout->addWidget(btn_favourite, 0, 2);
    layout->addWidget(lab_password, 1, 0);
    layout->addWidget(lne_password, 1, 1, 1, 2);
    layout->addWidget(btn_search, 0, 3, 2, 1);
    layout->addWidget(fra_separator, 2, 0, 1, 4);
    layout->addWidget(lab_stream, 3, 0, 1, 4);
    layout->addWidget(cbo_selector, 4, 0, 1, 4);
    layout->addLayout(layout_actions, 5, 0, 1, 4);
    layout->addWidget(btn_advanced, 6, 0, 1, 4);
    //layout->setRowStretch(3, 2);
    //layout->setRowStretch(4, 2);
    //layout->setRowStretch(5, 2);
    layout->setRowStretch(6, 1);

    setLayout(layout);

    connect(lne_channel, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(lne_channel, SIGNAL(textChanged(const QString &)), this, SLOT(verifyChannelFavouriting(const QString &)));
    connect(lne_password, SIGNAL(returnPressed()), this, SLOT(searchChannel()));
    connect(btn_favourite, SIGNAL(clicked()), this, SLOT(toggleChannelFavourite()));
    connect(btn_search, SIGNAL(clicked()), this, SLOT(searchChannel()));
    connect(cbo_selector, SIGNAL(currentIndexChanged(int)), p_live_channel, SLOT(setCurrentStream(int)));
    connect(btn_record, SIGNAL(clicked()), this, SIGNAL(gotoRecord()));
    connect(btn_watch, SIGNAL(clicked()), this, SIGNAL(gotoWatchAndStart()));
    connect(btn_advanced, SIGNAL(clicked()), this, SIGNAL(toggleUi()));
    //connect(p_live_channel, SIGNAL(messageChanged(const QString &)), this, SLOT(onMessageChanged(const QString &)));
    connect(p_live_channel, SIGNAL(channelSearchSuccess(QStringList)), this, SLOT(onSearchSuccess(QStringList)));
    connect(p_live_channel, SIGNAL(channelSearchError(const QString &)), this, SLOT(onSearchError(const QString &)));
}

void JtvLiveUiTabBasicJustin_tv::btn_watchEnable()
{
    btn_watch->setEnabled(true);
}

void JtvLiveUiTabBasicJustin_tv::btn_watchDisable()
{
    btn_watch->setDisabled(true);
}

void JtvLiveUiTabBasicJustin_tv::toggleChannelFavourite()
{
    QString ch = lne_channel->text();
    if(!ch.isEmpty())
    {
        btn_favouriteSetState(p_favourites->toggleFavourite(ch));
    }
}

void JtvLiveUiTabBasicJustin_tv::verifyChannelFavouriting(const QString &s)
{
    if(!s.isEmpty())
    {
        if(b_btn_favourite_isFav != p_favourites->isFavourited(s))
        {
            btn_favouriteSetState(!b_btn_favourite_isFav);
        }
    }
}

void JtvLiveUiTabBasicJustin_tv::searchChannel()
{
    if(lne_channel->text().isEmpty())
    {
        QMessageBox::information(this, "Search live channel", "Empty channel name field");
    }
    else
    {
        lock();
        cbo_selector->setDisabled(true);
        cbo_selector->clear();
        btn_record->setDisabled(true);
        btn_watch->setDisabled(true);
        p_live_channel->startSearch(lne_channel->text(), lne_password->text());
    }
}

/*void JtvLiveUiTabBasicJustin_tv::onMessageChanged(const QString &message)
{
    lab_infos->setText(message);
}*/

void JtvLiveUiTabBasicJustin_tv::onSearchSuccess(QStringList names)
{
    foreach(const QString &name, names)
    {
        cbo_selector->addItem(name);
    }
    cbo_selector->setCurrentIndex(0); //Will call Tab0_updateStreamDatas(int) [slot]
    cbo_selector->setEnabled(true);
    btn_record->setEnabled(true);
    btn_watch->setEnabled(true);
    unlock();
}

void JtvLiveUiTabBasicJustin_tv::onSearchError(const QString &error)
{
    QMessageBox::warning(this, "Search live channel", QString("An error occured : %1").arg(error));
    unlock();
}

void JtvLiveUiTabBasicJustin_tv::lock()
{
    lne_channel->setDisabled(true);
    btn_favourite->setDisabled(true);
    btn_search->setDisabled(true);
    lne_password->setDisabled(true);
}

void JtvLiveUiTabBasicJustin_tv::unlock()
{
    lne_channel->setEnabled(true);
    btn_favourite->setEnabled(true);
    btn_search->setEnabled(true);
    lne_password->setEnabled(true);
}

void JtvLiveUiTabBasicJustin_tv::btn_favouriteSetState(bool isFav)
{
    b_btn_favourite_isFav = isFav;
    if(b_btn_favourite_isFav)
    {
        btn_favourite->setIcon(QIcon(":img/delete.png"));
    }
    else
    {
        btn_favourite->setIcon(QIcon(":img/heart.png"));
    }
}
