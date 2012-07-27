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

#include "JtvLiveChannel.h"

#define implementGetOnStream(getSuffix, streamMember) \
QString JtvLiveChannel::getStream##getSuffix() const\
{\
    return (i_current_stream >= 0 && i_current_stream < l_streams.size()) ? l_streams.at(i_current_stream).streamMember : QString();\
}

#define implementSetOnStream(setSuffix, streamMember) \
void JtvLiveChannel::setStream##setSuffix(const QString &param)\
{\
    if(i_current_stream >= 0 && i_current_stream < l_streams.size())\
    {\
        l_streams[i_current_stream].streamMember = param;\
        emit paramsChanged();\
    }\
}

JtvLiveChannel::JtvLiveChannel(QNetworkAccessManager *network_manager, const QString &base_palyer_url, const QString &base_http_referer, QObject *parent) :
    QObject(parent)
{
    //pl_streams = new QList<JtvLiveStream>;
    p_net_manager = network_manager;
    p_net_reply = 0;
    i_current_stream = -1;
    //Retrieving dynamic player URL
    qs_http_referer = base_http_referer;
    qs_player_url = base_palyer_url;
    QNetworkRequest req = QNetworkRequest(QUrl(qs_player_url));
    req.setRawHeader("Referer", qs_http_referer.toAscii());
    req.setPriority(QNetworkRequest::HighPriority);
    p_player_reply = p_net_manager->get(req);
    connect(p_player_reply, SIGNAL(finished()), this, SLOT(gotPlayerRedirect()));
}

void JtvLiveChannel::logMessage(const QString &message)
{
    if(!message.isEmpty())
    {
        qs_last_message = message;
        emit messageChanged(message);
    }
}

const QString & JtvLiveChannel::getLastMessage() const
{
    return qs_last_message;
}

const QString & JtvLiveChannel::getHttpReferer() const
{
    return qs_http_referer;
}

const QString & JtvLiveChannel::getPlayerUrl() const
{
    return qs_player_url;
}

QStringList JtvLiveChannel::getStreamsDisplayName() const
{
    QStringList list;
    int streams_size = l_streams.size();
    for(int i = 0 ; i < streams_size ; i++)
    {
        QString name = QString("%1\t\tHeight : %2\t\t").arg(l_streams.at(i).tag_name, l_streams.at(i).height);
        if(l_streams.at(i).server_type == JtvLiveStream::UsherServer)
        {
            name.append("UsherToken");
        }
        else if(l_streams.at(i).server_type == JtvLiveStream::AkamaiServer)
        {
            name.append("SWF Vfy");
        }
        list.append(name);
    }
    return list;
}

/* Parameters getters/generators */
implementGetOnStream(Rtmp, rtmp_url)
implementGetOnStream(Swf, swf)
implementGetOnStream(SwfVfy, swf_vfy)
implementGetOnStream(UsherToken, usher_token)
implementGetOnStream(Web, web)
/***********************************/

/* Stream stats accessors */
implementGetOnStream(TagName, tag_name)
implementGetOnStream(Height, height)
implementGetOnStream(Bitrate, bitrate)
implementGetOnStream(Part, part)
implementGetOnStream(Id, id)
implementGetOnStream(Viewers, viewers)
implementGetOnStream(Node, node)
/**************************/

QStringList JtvLiveChannel::getRtmpParams() const
{
    QStringList args;
    QString a;
    a = getStreamRtmp();
    if(!a.isEmpty())
    {
        args << "-r";
        args << a;
    }
    a = getStreamSwf();
    if(!a.isEmpty())
    {
        args << "-s";
        args << a;
    }
    a = getStreamSwfVfy();
    if(!a.isEmpty())
    {
        args << "-W";
        args << a;
    }
    a = getStreamWeb();
    if(!a.isEmpty())
    {
        args << "-p";
        args << a;
    }
    a = getStreamUsherToken();
    if(!a.isEmpty())
    {
        args << "-j";
        args << a;
    }
    args << "-v";
    return args;
}

QString JtvLiveChannel::escape4CLI(QStringList args)
{
    args.replaceInStrings("\"", "\\\"");
#ifndef Q_OS_WIN
    args.replaceInStrings("&", "\\&"); //& is escaped by backslash under Linux
#endif
    int s = args.size();
    QString temp;
    for (int i = 0 ; i < s ; ++i)
    {
        temp = args.at(i);
#ifdef Q_OS_WIN
        if(temp.contains(' ') || temp.contains('&')) //surrounding & by quotes in enough under Windows
#else
        if(temp.contains(' '))
#endif
        {
            temp.prepend('"');
            temp.append('"');
            args.replace(i, temp);
        }
    }
    return args.join(" ");
}

void JtvLiveChannel::startSearch(const QString &channel, const QString &password)
{
    if(p_net_reply == 0)
    {
        if(channel.isEmpty())
        {
            logMessage("Empty parameter: channel");
            emit channelSearchError("Channel name can't be empty!");
        }
        else
        {
            i_current_stream = -1;
            l_streams.clear();
            qs_channel_name = channel;
            QUrl url(QString("http://usher.justin.tv/find/%1.xml").arg(qs_channel_name));
            url.addQueryItem("type", "any");
            url.addQueryItem("p", QString("%1%2%3%4%5%6").arg(qrand() % 10).arg(qrand() % 10).arg(qrand() % 10).arg(qrand() % 10).arg(qrand() % 10).arg(qrand() % 10));
            url.addQueryItem("group", "");
            url.addQueryItem("b_id", "true");
            if(password.isEmpty())
            {
                url.addQueryItem("private_code", "null");
            }
            else
            {
                url.addQueryItem("private_code", password);
            }
            logMessage(url.toString());
            QNetworkRequest req;
            req.setUrl(url);
            logMessage(QString("Downloading: %1").arg(url.toString()));
            p_net_reply = p_net_manager->get(req);
            connect(p_net_reply, SIGNAL(finished()), this, SLOT(dlFinished()));
        }
    }
    else
    {
        logMessage("Search already running!");
    }
}

void JtvLiveChannel::setCurrentStream(int i)
{
    if(i >= 0 && i < l_streams.size())
    {
        i_current_stream = i;
        emit streamChanged();
        emit paramsChanged();
    }
}

/* Parameters setters */
implementSetOnStream(Rtmp, rtmp_url)
implementSetOnStream(Swf, swf)
implementSetOnStream(SwfVfy, swf_vfy)
implementSetOnStream(UsherToken, usher_token)
implementSetOnStream(Web, web)
/**********************/

void JtvLiveChannel::gotPlayerRedirect()
{
    disconnect(p_player_reply, SIGNAL(finished()), this, SLOT(gotPlayerRedirect()));
    QVariant loc = p_player_reply->header(QNetworkRequest::LocationHeader);
    if(loc.isValid())
    {
        qs_player_url = loc.toString();
    }
    p_player_reply->deleteLater();
    p_player_reply = 0;
}

void JtvLiveChannel::dlFinished()
{
    disconnect(p_net_reply, SIGNAL(finished()), this, SLOT(dlFinished()));
    QNetworkReply::NetworkError error = p_net_reply->error();
    if(error == QNetworkReply::NoError)
    {
        logMessage("Download finished without error");
        logMessage("Now, parsing this nice XML...");
        parseXml(p_net_reply->readAll());
    }
    //TODO : Implement error cases from QNetworkReply::NetworkError
    else
    {
        logMessage("There was an error somewhere");
        //logMessage("Emitting channelSearchError(QString)");
        emit channelSearchError("Network error");
    }
    p_net_reply->deleteLater();
    p_net_reply = 0;
}

void JtvLiveChannel::parseXml(const QByteArray &raw_datas)
{
    QString clean_datas(raw_datas);
    //Workaround for invalid Jtv XML !
    clean_datas.replace(QRegExp("<(\\d+p)>"), "<live-\\1>");
    clean_datas.replace(QRegExp("</(\\d+p)>"), "</live-\\1>");
    QDomDocument dom;
    if(dom.setContent(clean_datas))
    {
        QDomElement nodes = dom.documentElement();
        if(nodes.hasChildNodes())
        {
            for(QDomNode stream = nodes.firstChild() ; !stream.isNull() ; stream = stream.nextSibling())
            {
                JtvLiveStream live_stream;
                //live_stream.channel_name = qs_channel_name;
                QDomElement t = stream.toElement();
                logMessage(QString("Found <%1> node ...").arg(t.tagName()));
                live_stream.tag_name = t.tagName();
                QDomNode n;
                //Creating display_name : tagname + <video_height>
                n = t.elementsByTagName("video_height").item(0);
                if(!n.isNull())
                {
                    live_stream.height = n.toElement().text().trimmed();
                    //live_stream.display_name = QString("%1\tHeight : %2").arg(t.tagName(), n.toElement().text().trimmed());
                    //Creating rtmp_url : <connect> + <play>
                    n = t.elementsByTagName("connect").item(0);
                    if(!n.isNull())
                    {
                        QString v = n.toElement().text().trimmed();
                        n = t.elementsByTagName("play").item(0);
                        if(!n.isNull())
                        {
                            live_stream.rtmp_url = v.append("/").append(n.toElement().text().trimmed());
                            //<token>
                            n = t.elementsByTagName("token").item(0);
                            if(!n.isNull())
                            {
                                live_stream.usher_token = n.toElement().text().trimmed();
                                //<bitrate>
                                n = t.elementsByTagName("bitrate").item(0);
                                if(!n.isNull())
                                {
                                    live_stream.bitrate = n.toElement().text().trimmed();
                                    //<broadcast_part>
                                    n = t.elementsByTagName("broadcast_part").item(0);
                                    if(!n.isNull())
                                    {
                                        live_stream.part = n.toElement().text().trimmed();
                                        //<broadcast_id>
                                        n = t.elementsByTagName("broadcast_id").item(0);
                                        if(!n.isNull())
                                        {
                                            live_stream.id = n.toElement().text().trimmed();
                                            //<viewer_count>
                                            //NOTE: ams01 cluster doesn't provide viewer_count [2707/2012]
                                            n = t.elementsByTagName("viewer_count").item(0);
                                            if(n.isNull())
                                            {
                                                live_stream.viewers = QString("unknown");
                                            }
                                            else
                                            {
                                                live_stream.viewers = n.toElement().text().trimmed();
                                            }
                                            //<node> -> server_type
                                            n = t.elementsByTagName("node").item(0);
                                            if(!n.isNull())
                                            {
                                                QString v = n.toElement().text().trimmed();
                                                live_stream.node = v;
                                                if(v.toLower() == "akamai")
                                                {
                                                    live_stream.server_type = JtvLiveStream::AkamaiServer;
                                                    live_stream.swf_vfy = qs_player_url;
                                                }
                                                else
                                                {
                                                    live_stream.server_type = JtvLiveStream::UsherServer;
                                                    live_stream.swf_vfy = QString();
                                                }
                                                live_stream.swf = qs_player_url;
                                                live_stream.web = QString(qs_http_referer).append(qs_channel_name);
                                                //Adding the live_stream struct in streams QList, parsing stream node finished
                                                l_streams.append(live_stream);
                                            }
                                            else
                                            {
                                                emit channelSearchError("8");
                                            }

                                        }
                                        else
                                        {
                                            emit channelSearchError("7");
                                        }
                                    }
                                    else
                                    {
                                        emit channelSearchError("6");
                                    }
                                }
                                else
                                {
                                    emit channelSearchError("5");
                                }
                            }
                            else
                            {
                                emit channelSearchError("4");
                            }
                        }
                        else
                        {
                            emit channelSearchError("3");
                        }
                    }
                    else
                    {
                        emit channelSearchError("2");
                    }
                }
                else
                {
                    emit channelSearchError("1");
                }
            }
            //If the list is empty, there was no valid stream node
            if(l_streams.isEmpty())
            {
                logMessage("The stream list is empty: Jtv XML is broken or this class is outdated!");
                emit channelSearchError("There was no valid stream node");
            }
            else
            {
                logMessage(QString("%1 stream(s) in list, parsing finished.").arg(l_streams.size()));
                emit channelSearchSuccess(getStreamsDisplayName());
            }
        }
        else
        {
            logMessage("Empty <nodes>");
            emit channelSearchError("This channel isn't live or doesn't exist");
        }
    }
    else
    {
        logMessage("Failed to parse datas as DOM tree!");
        emit channelSearchError("Failed to parse datas as DOM tree");
    }
}

JtvLiveChannel::~JtvLiveChannel()
{ }
