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

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QList>
#include <QStringList>
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
    QString swf, swf_vfy, web, tag_name, height, rtmp_url, usher_token, bitrate, part, id, viewers, node;
    JtvServerType server_type;
};

class JtvLiveChannel : public QObject
{
    Q_OBJECT

public:
    explicit JtvLiveChannel(QNetworkAccessManager *net_manager, const QString &base_player_url = "http://fr.justin.tv/widgets/live_embed_player.swf", const QString &base_http_referer = "http://fr.justin.tv/", QObject *parent = 0);
    ~JtvLiveChannel();
    bool isReady() const;
    const QString & getLastMessage() const;
    const QString & getHttpReferer() const;
    const QString & getPlayerUrl() const;
    QStringList getStreamsDisplayName() const;
    /* Parameters getters/generators */
    QString getStreamRtmp() const;
    QString getStreamSwf() const;
    QString getStreamSwfVfy() const;
    QString getStreamUsherToken() const;
    QString getStreamWeb() const;
    /***********************************/
    /* Stream stats accessors */
    QString getStreamTagName() const;
    QString getStreamHeight() const;
    QString getStreamBitrate() const;
    QString getStreamPart() const;
    QString getStreamId() const;
    QString getStreamViewers() const;
    QString getStreamNode() const;
    /**************************/
    QStringList getRtmpParams() const;
    static QString escape4CLI(QStringList args);

signals:
    void streamChanged();
    void paramsChanged();
    void messageChanged(const QString &);
    void channelSearchError(const QString &);
    void channelSearchSuccess(QStringList);

public slots:
    void startSearch(const QString &channel, const QString &password);
    void setCurrentStream(int i);
    /* Parameters setters */
    void setStreamRtmp(const QString &param);
    void setStreamSwf(const QString &param);
    void setStreamSwfVfy(const QString &param);
    void setStreamUsherToken(const QString &param);
    void setStreamWeb(const QString &param);
    /**********************/

protected slots:
    void gotPlayerRedirect();
    void dlFinished();

protected:
    void logMessage(const QString &message);
    void parseXml(const QByteArray &raw_datas);

    QString qs_channel_name, qs_player_url, qs_http_referer, qs_last_message;
    QList<JtvLiveStream> l_streams;
    int i_current_stream;
    QNetworkAccessManager *p_net_manager;
    QNetworkReply *p_net_reply, *p_player_reply;
};

#endif // JTVLIVECHANNEL_H
