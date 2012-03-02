#ifndef JTVLIVECHANNEL_H
#define JTVLIVECHANNEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QByteArray>

//Hardcoding is BAD !
#define JTV_PLAYER "http://fr.justin.tv/widgets/live_embed_player.swf"

class JtvLiveStream
{
public:
    enum JtvServerType {UsherServer, AkamaiServer};
    QString channel_name, player_url, tag_name, height, rtmp_url, usher_token, bitrate, part, id, viewers, node;
    JtvLiveStream::JtvServerType server_type;
};

class JtvLiveChannel : public QObject
{
    Q_OBJECT

public:
    explicit JtvLiveChannel(QObject *parent = 0);
    ~JtvLiveChannel();
    const QString & getLastMessage() const;
    QList<JtvLiveStream>* getStreams();
    void startSearch(const QString &channel);

signals:
    void messageChanged(const QString &);
    void channelSearchError(const QString &);
    void channelSearchSuccess(QList<JtvLiveStream> *);

protected slots:
    void dlFinished(QNetworkReply *reply);

protected:
    void logMessage(const QString &message);
    void parseXml(const QByteArray &raw_datas);

    QString channel_name;
    QString player_url;
    QList<JtvLiveStream> *streams;
    QString last_message;
    QNetworkAccessManager *net_manager;

};

#endif // JTVLIVECHANNEL_H
