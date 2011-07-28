#ifndef JTVLIVECHANNEL_H
#define JTVLIVECHANNEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QByteArray>

//Hardcoding is BAD !
#define JTV_PLAYER "http://fr.justin.tv/widgets/live_embed_player.swf"

enum JtvServerType
{
    LEGACY, AKAMAI
};

typedef struct JtvLiveStream JtvLiveStream;
struct JtvLiveStream
{
    QString channel_name, player_url, tag_name, height, rtmp_url, usher_token, bitrate, part, id, viewers, node;
    JtvServerType server_type;
};

class JtvLiveChannel : public QObject
{
    Q_OBJECT

public:
    explicit JtvLiveChannel(QObject *parent = 0);
    ~JtvLiveChannel();
    QString getLastMessage() const;
    QList<JtvLiveStream>* getStreams();
    void startSearch(QString channel);

signals:
    void messageChanged(QString);
    void channelSearchError(QString);
    void channelSearchSuccess(QList<JtvLiveStream>*);

public slots:

protected slots:
    void dlFinished(QNetworkReply *reply);

protected:
    QString channel_name;
    QString player_url;
    QList<JtvLiveStream> *streams;
    QString last_message;

    QNetworkAccessManager *net_manager;
    void logMessage(QString message);
    void parseXml(QByteArray raw_datas);
};

#endif // JTVLIVECHANNEL_H
