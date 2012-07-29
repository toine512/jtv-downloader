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

#include "JtvLiveUiTabParams.h"

JtvLiveUiTabParams::JtvLiveUiTabParams(JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_live_channel = live_channel;

    lne_rtmp = new QLineEdit;
    lab_rtmp = new QLabel("RTMP (-r)");
    lab_rtmp->setBuddy(lne_rtmp);

    lne_swf = new QLineEdit;
    lab_swf = new QLabel("SWF (-s)");
    lab_swf->setBuddy(lne_swf);

    lne_swf_vfy = new QLineEdit;
    lab_swf_vfy = new QLabel("SWF verif (-W)");
    lab_swf_vfy->setBuddy(lne_swf_vfy);

    lne_web = new QLineEdit;
    lab_web = new QLabel("Web page (-p)");
    lab_web->setBuddy(lne_web);

    lne_usher_token = new QLineEdit;
    lab_usher_token = new QLabel("UsherToken (-j)");
    lab_usher_token->setBuddy(lne_usher_token);

    fra_separator = new QFrame;
    fra_separator->setFrameShape(QFrame::HLine);
    fra_separator->setFrameShadow(QFrame::Sunken);

    lab_cli = new QLabel("Command line input firendly :");
    pte_cli = new QPlainTextEdit;
    pte_cli->setReadOnly(true);

    //Layout
    layout = new QGridLayout;
    layout->addWidget(lab_rtmp, 0, 0);
    layout->addWidget(lne_rtmp, 0, 1);
    layout->addWidget(lab_swf, 1, 0);
    layout->addWidget(lne_swf, 1, 1);
    layout->addWidget(lab_swf_vfy, 2, 0);
    layout->addWidget(lne_swf_vfy, 2, 1);
    layout->addWidget(lab_web, 3, 0);
    layout->addWidget(lne_web, 3, 1);
    layout->addWidget(lab_usher_token, 4, 0);
    layout->addWidget(lne_usher_token, 4, 1);
    layout->addWidget(fra_separator, 5, 0, 1, 2);
    layout->addWidget(lab_cli, 6, 0, 1, 2);
    layout->addWidget(pte_cli, 7, 0, 1, 2);

    setLayout(layout);

    connect(lne_rtmp, SIGNAL(textEdited(const QString &)), p_live_channel, SLOT(setStreamRtmp(const QString &)));
    connect(lne_swf, SIGNAL(textEdited(const QString &)), p_live_channel, SLOT(setStreamSwf(const QString &)));
    connect(lne_swf_vfy, SIGNAL(textEdited(const QString &)), p_live_channel, SLOT(setStreamSwfVfy(const QString &)));
    connect(lne_web, SIGNAL(textEdited(const QString &)), p_live_channel, SLOT(setStreamWeb(const QString &)));
    connect(lne_usher_token, SIGNAL(textEdited(const QString &)), p_live_channel, SLOT(setStreamUsherToken(const QString &)));
    connect(p_live_channel, SIGNAL(streamChanged()), this, SLOT(setParams()));
    connect(p_live_channel, SIGNAL(paramsChanged()), this, SLOT(updateCli()));
}

void JtvLiveUiTabParams::clearParams()
{
    lne_rtmp->clear();
    lne_swf->clear();
    lne_swf_vfy->clear();
    lne_web->clear();
    lne_usher_token->clear();
    pte_cli->clear();
}

void JtvLiveUiTabParams::setParams()
{
    lne_rtmp->setText(p_live_channel->getStreamRtmp());
    lne_swf->setText(p_live_channel->getStreamSwf());
    lne_swf_vfy->setText(p_live_channel->getStreamSwfVfy());
    lne_web->setText(p_live_channel->getStreamWeb());
    lne_usher_token->setText(p_live_channel->getStreamUsherToken());
}

void JtvLiveUiTabParams::updateCli()
{
    pte_cli->setPlainText(JtvLiveChannel::escape4CLI(p_live_channel->getRtmpParams()));
}
