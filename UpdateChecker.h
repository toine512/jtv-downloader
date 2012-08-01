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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QtGlobal>
#include <QObject>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

/* 0x 0A 00 00 00 = alpha
   0x 0B 00 00 00 = beta
   0x 0F 00 00 00 = release */
#define UPDATE_MASK_ALPHA 0x0A000000
#define UPDATE_MASK_BETA 0x0B000000
#define UPDATE_MASK_RELEASE 0x0F000000

class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QNetworkAccessManager *network_manager, const QString &base_url, const QString &client_uuid, quint32 version, QObject *parent = 0);
    static QString whichRelease(quint32 version);
    
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
