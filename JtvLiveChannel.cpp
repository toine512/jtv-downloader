#include "JtvLiveChannel.h"
#include <QNetworkRequest>
#include <cstdlib>
#include <QMessageBox>

JtvLiveChannel::JtvLiveChannel(QObject *parent) :
    QObject(parent)
{
    //get a preferences object (?) and set the Jtv player URL
    player_url.fromUtf8(JTV_PLAYER); //using hardcoded #define (see header)
    streams = new QVector<JtvLiveStream>;
    net_manager = new QNetworkAccessManager(this);
}

void JtvLiveChannel::logMessage(QString message)
{
    if(!message.isEmpty())
    {
        last_message = message;
        emit messageChanged(message);
    }
}

QString JtvLiveChannel::getLastMessage() const
{
    return last_message;
}

QVector<JtvLiveStream>* JtvLiveChannel::getStreams()
{
    return streams;
}

void JtvLiveChannel::startSearch(QString channel)
{
    streams->clear();
    channel_name = channel;
    QUrl url(QString("http://usher.justin.tv/find/%1.xml").arg(channel_name));
    url.addQueryItem("type", "any");
    url.addQueryItem("p", QString("%1%2%3%4%5%6").arg(rand() % 10).arg(rand() % 10).arg(rand() % 10).arg(rand() % 10).arg(rand() % 10).arg(rand() % 10)); //TODO : using srand() ...
    url.addQueryItem("group", "");
    url.addQueryItem("channel_subscription", "");
    logMessage(url.toString());
    QNetworkRequest req;
    req.setUrl(url);
    connect(net_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dlFinished(QNetworkReply*)));
    logMessage(QString("Downloading : %1").arg(url.toString()));
    net_manager->get(req);
}

void JtvLiveChannel::dlFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError error = reply->error();
    if(error == QNetworkReply::NoError)
    {
        logMessage("Download finished without error");
    }
    //TODO : Implement error cases from QNetworkReply::NetworkError
    else
    {
        logMessage("There was an error somewhere");
        logMessage("Emitting channelSearchError(QString)");
        emit channelSearchError("There was an error somewhere");
    }
    reply->deleteLater();
}

JtvLiveChannel::~JtvLiveChannel()
{
    //delete last_message;
    delete streams;
}
