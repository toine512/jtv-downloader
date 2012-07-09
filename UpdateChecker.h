#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QNetworkAccessManager *network_manager, const QString &base_url, const QString &client_uuid, quint32 version, QObject *parent = 0);
    
signals:
    void updateAvailable(const QString &new_version_human, const QString &dl_link);
    void updateNotes(const QString &notes);
    
protected slots:
    void checkDone();
    void notesDone();

protected:
    QNetworkAccessManager *net_manager;
    QNetworkReply *net_reply;
    QString url;
    quint32 current_version;
};

#endif // UPDATECHECKER_H
