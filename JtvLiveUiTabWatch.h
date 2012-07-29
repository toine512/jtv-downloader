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

#ifndef JTVLIVEUITABWATCH_H
#define JTVLIVEUITABWATCH_H

#include <QSettings>
#include <QString>
#include <QProcess>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

class JtvLiveUiTabWatch : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabWatch(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent = 0);
    
signals:
    void askBtn_watchEnable();
    void askBtn_watchDisable();
    
public slots:
    void linkedProcessesStart();

protected slots:
    void settingsSetPlayerPath(const QString &path);
    void linkedProcessesError(const QProcess::ProcessError &error);
    void linkedProcessesDisconnectTerminate();
    void rtmpgwOut();
    void playerOut();

protected:
    void linkedProcessesTerminate();

    QSettings *p_settings;
    JtvLiveChannel *p_live_channel;
    QProcess *qproc_rtmpgw, *qproc_player;
    
    QLabel *lab_player;
    QLineEdit *lne_player;
    QPushButton *btn_watch;
    QFrame *fra_separator;
    QPlainTextEdit *pte_rtmpgw, *pte_player;
    QHBoxLayout *layout_player;
    QVBoxLayout *layout;
};

#endif // JTVLIVEUITABWATCH_H
