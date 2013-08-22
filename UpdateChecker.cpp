/* This file is part of "Jtv live downloader"
 *
 * Copyright (C) 2012-2013 toine512 <toine512@gmail.com>
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

#include "UpdateChecker.h"

UpdateChecker::UpdateChecker(QNetworkAccessManager *network_manager, const QString &base_url, const QString &client_uuid, quint32 version, QObject *parent) :
    QObject(parent)
{
    net_manager = network_manager;
    url = base_url;
    current_version = version;

    QNetworkRequest req = QNetworkRequest(QString(url).append("latest.release"));
    req.setPriority(QNetworkRequest::LowPriority);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrl data;
    data.addQueryItem("uuid", client_uuid);
    data.addQueryItem("version", QString::number(current_version));
    net_reply = net_manager->post(req, data.encodedQuery());
    connect(net_reply, SIGNAL(finished()), this, SLOT(checkDone()));
}

QString UpdateChecker::whichRelease(quint32 version)
{
    if((version & UPDATE_MASK_RELEASE) == UPDATE_MASK_RELEASE)
    {
        return QString("release");
    }
    else if((version & UPDATE_MASK_BETA) == UPDATE_MASK_BETA)
    {
        return QString("beta");
    }
    else if((version & UPDATE_MASK_ALPHA) == UPDATE_MASK_ALPHA)
    {
        return QString("alpha");
    }
    else
    {
        return QString("unknown");
    }
}

void UpdateChecker::checkDone()
{
    disconnect(net_reply, SIGNAL(finished()), this, SLOT(checkDone()));
    if(net_reply->error() == QNetworkReply::NoError)
    {
        QString raw = QString(net_reply->readAll());
        net_reply->deleteLater();
        bool ok;
        quint32 ver = raw.section(',', 0, 0).toUInt(&ok, 16);
        if(ok == true && ver > current_version)
        {
            emit updateAvailable(raw.section(',', 1, 1), raw.section(',', 2));

            QNetworkRequest req = QNetworkRequest(QUrl(QString(url).append("notes.release")));
            req.setPriority(QNetworkRequest::LowPriority);
            net_reply = net_manager->get(req);
            connect(net_reply, SIGNAL(finished()), this, SLOT(notesDone()));
        }
        else
        {
            net_reply = 0;
        }
    }
}

void UpdateChecker::notesDone()
{
    disconnect(net_reply, SIGNAL(finished()), this, SLOT(notesDone()));
    if(net_reply->error() == QNetworkReply::NoError)
    {
        emit updateNotes(QString(net_reply->readAll()));
        net_reply->deleteLater();
        net_reply = 0;
    }
}
