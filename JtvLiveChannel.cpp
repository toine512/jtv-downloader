#include "JtvLiveChannel.h"
#include <cstdlib>
#include <QUrl>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QNetworkReply>
#include <QRegExp>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

JtvLiveChannel::JtvLiveChannel(QNetworkAccessManager *network_manager, QObject *parent) :
    QObject(parent)
{
    streams = new QList<JtvLiveStream>;
    net_manager = network_manager;
    net_reply = 0;
}

void JtvLiveChannel::logMessage(const QString &message)
{
    if(!message.isEmpty())
    {
        last_message = message;
        emit messageChanged(message);
    }
}

const QString & JtvLiveChannel::getLastMessage() const
{
    return last_message;
}

QList<JtvLiveStream>* JtvLiveChannel::getStreams()
{
    return streams;
}

void JtvLiveChannel::startSearch(const QString &channel)
{
    streams->clear();
    channel_name = channel;
    QUrl url(QString("http://usher.justin.tv/find/%1.xml").arg(channel_name));
    url.addQueryItem("type", "any");
    url.addQueryItem("p", QString("%1%2%3%4%5%6").arg(rand() % 10).arg(rand() % 10).arg( rand() % 10).arg(rand() % 10).arg(rand() % 10).arg(rand() % 10)); //TODO : use srand() ...
    url.addQueryItem("group", "");
    url.addQueryItem("b_id", "true");
    url.addQueryItem("private_code", "null");
    logMessage(url.toString());
    QNetworkRequest req;
    req.setUrl(url);
    logMessage(QString("Downloading : %1").arg(url.toString()));
    net_reply = net_manager->get(req);
    connect(net_reply, SIGNAL(finished()), this, SLOT(dlFinished()));
}

void JtvLiveChannel::dlFinished()
{
    disconnect(net_reply, SIGNAL(finished()), this, SLOT(dlFinished()));
    QNetworkReply::NetworkError error = net_reply->error();
    if(error == QNetworkReply::NoError)
    {
        logMessage("Download finished without error");
        logMessage("Now, parsing this nice XML ...");
        parseXml(net_reply->readAll());
    }
    //TODO : Implement error cases from QNetworkReply::NetworkError
    else
    {
        logMessage("There was an error somewhere");
        //logMessage("Emitting channelSearchError(QString)");
        emit channelSearchError("Network error");
    }
    net_reply->deleteLater();
    net_reply = 0;
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
                live_stream.channel_name = channel_name;
                QDomElement t = stream.toElement();
                logMessage(QString("Found <%1> node ...").arg(t.tagName()));
                live_stream.tag_name = t.tagName();
                //Creating display_name : tagname + <video_height>
                QDomNode n = t.elementsByTagName("video_height").item(0);
                if(!n.isNull())
                {
                    live_stream.height = n.toElement().text().trimmed();
                    //live_stream.display_name = QString("%1\tHeight : %2").arg(t.tagName(), n.toElement().text().trimmed());
                    //Creating rtmp_url : <connect> + <play>
                    QDomNode n = t.elementsByTagName("connect").item(0);
                    if(!n.isNull())
                    {
                        QString v = n.toElement().text().trimmed();
                        QDomNode n = t.elementsByTagName("play").item(0);
                        if(!n.isNull())
                        {
                            live_stream.rtmp_url = v.append("/").append(n.toElement().text().trimmed());
                            //<token>
                            QDomNode n = t.elementsByTagName("token").item(0);
                            if(!n.isNull())
                            {
                                live_stream.usher_token = n.toElement().text().trimmed();
                                //<bitrate>
                                QDomNode n = t.elementsByTagName("bitrate").item(0);
                                if(!n.isNull())
                                {
                                    live_stream.bitrate = n.toElement().text().trimmed();
                                    //<broadcast_part>
                                    QDomNode n = t.elementsByTagName("broadcast_part").item(0);
                                    if(!n.isNull())
                                    {
                                        live_stream.part = n.toElement().text().trimmed();
                                        //<broadcast_id>
                                        QDomNode n = t.elementsByTagName("broadcast_id").item(0);
                                        if(!n.isNull())
                                        {
                                            live_stream.id = n.toElement().text().trimmed();
                                            //<viewer_count>
                                            QDomNode n = t.elementsByTagName("viewer_count").item(0);
                                            if(!n.isNull())
                                            {
                                                live_stream.viewers = n.toElement().text().trimmed();
                                                //<node> -> server_type
                                                QDomNode n = t.elementsByTagName("node").item(0);
                                                if(!n.isNull())
                                                {
                                                    QString v = n.toElement().text().trimmed();
                                                    live_stream.node = v;
                                                    if(v.toLower() == "akamai")
                                                    {
                                                        live_stream.server_type = JtvLiveStream::AkamaiServer;
                                                    }
                                                    else
                                                    {
                                                        live_stream.server_type = JtvLiveStream::UsherServer;
                                                    }
                                                    //Adding the live_stream struct in streams QList, parsing stream node finished
                                                    streams->append(live_stream);
                                                }
                                                else
                                                {
                                                    logMessage("");
                                                    emit channelSearchError("");
                                                }
                                             }
                                            else
                                            {
                                                logMessage("");
                                                emit channelSearchError("");
                                            }
                                        }
                                        else
                                        {
                                            logMessage("");
                                            emit channelSearchError("");
                                        }
                                    }
                                    else
                                    {
                                        logMessage("");
                                        emit channelSearchError("");
                                    }
                                }
                                else
                                {
                                    logMessage("");
                                    emit channelSearchError("");
                                }
                            }
                            else
                            {
                                logMessage("");
                                emit channelSearchError("");
                            }
                        }
                        else
                        {
                            logMessage("");
                            emit channelSearchError("");
                        }
                    }
                    else
                    {
                        logMessage("");
                        emit channelSearchError("");
                    }
                }
                else
                {
                    logMessage("");
                    emit channelSearchError("");
                }
            }
            //If the list is empty, there was no valid stream node
            if(streams->isEmpty())
            {
                logMessage("The stream list is empty !");
                emit channelSearchError("There was no valid stream node");
            }
            else
            {
                logMessage(QString("%1 stream(s) in list, parsing finished.").arg(streams->size()));
                emit channelSearchSuccess(streams);
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
        logMessage("Failed to parse datas as DOM tree !");
        emit channelSearchError("Failed to parse datas as DOM tree");
    }
}

JtvLiveChannel::~JtvLiveChannel()
{
    delete streams;
}
