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

#ifndef JTVLIVECHANNEL_H
#define JTVLIVECHANNEL_H

#include <cstdlib>

#include <QObject>
#include <QString>
#include <QUrl>
#include <QList>
#include <QByteArray>
#include <QRegExp>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

class JtvLiveStream
{
public:
    enum JtvServerType {UsherServer, AkamaiServer};
    QString channel_name, tag_name, height, rtmp_url, usher_token, bitrate, part, id, viewers, node;
    JtvServerType server_type;
};

class JtvLiveChannel : public QObject
{
    Q_OBJECT

public:
    explicit JtvLiveChannel(QNetworkAccessManager *net_manager, const QString &base_player_url = "http://fr.justin.tv/widgets/live_embed_player.swf", const QString &base_http_referer = "http://fr.justin.tv/", QObject *parent = 0);
    ~JtvLiveChannel();
    const QString & getLastMessage() const;
    const QString & getHttpReferer() const;
    const QString & getPlayerUrl() const;
    QList<JtvLiveStream>* getStreams();
    void startSearch(const QString &channel, const QString &password);

signals:
    void messageChanged(const QString &);
    void channelSearchError(const QString &);
    void channelSearchSuccess(QList<JtvLiveStream> *);

protected slots:
    void gotPlayerRedirect();
    void dlFinished();

protected:
    void logMessage(const QString &message);
    void parseXml(const QByteArray &raw_datas);

    QString channel_name, player_url, http_referer, last_message;
    QList<JtvLiveStream> *streams;
    QNetworkAccessManager *net_manager;
    QNetworkReply *net_reply, *player_reply;
};

#endif // JTVLIVECHANNEL_H
