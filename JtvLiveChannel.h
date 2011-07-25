#ifndef JTVLIVECHANNEL_H
#define JTVLIVECHANNEL_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkReply>
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
    QString display_name, channel_name, rtmp_url, usher_token, player_url;
    JtvServerType server_type;
    //Misc infos
    /* int or QString, depending on the XML parsing ... */
    QString bitrate, viewers, part;
    QString id;
    /***/
    QString node;
};

class JtvLiveChannel : public QObject
{
    Q_OBJECT

public:
    explicit JtvLiveChannel(QObject *parent = 0);
    ~JtvLiveChannel();
    QString getLastMessage() const;
    QVector<JtvLiveStream>* getStreams();
    void startSearch(QString channel);

signals:
    void messageChanged(QString);
    void channelSearchError(QString);
    void channelSearchSuccess(QVector<JtvLiveStream>*);

public slots:

protected slots:
    void dlFinished(QNetworkReply *reply);

protected:
    QString channel_name;
    QString player_url;
    QVector<JtvLiveStream> *streams;
    QString last_message;

    QNetworkAccessManager *net_manager;
    void logMessage(QString message);
    void parseXml(QByteArray raw_datas);
};

#endif // JTVLIVECHANNEL_H
