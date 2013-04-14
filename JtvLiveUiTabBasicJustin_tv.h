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

#ifndef JTVLIVEUITABBASICJUSTIN_TV_H
#define JTVLIVEUITABBASICJUSTIN_TV_H

#include <QString>
#include <QStringList>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include "qt-shared/buttons/QSquareIconResizingPushButton.h"

class JtvLiveUiTabBasicJustin_tv : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabBasicJustin_tv(JtvLiveChannel *live_channel, QWidget *parent = 0);

signals:
    void toggleUi();
    void gotoRecord();
    void gotoWatchAndStart();

public slots:
    void btn_watchEnable();
    void btn_watchDisable();

protected slots:
    void searchChannel();
    //void onMessageChanged(const QString &message);
    void onSearchSuccess(QStringList names);
    void onSearchError(const QString &error);

protected:
    void lock();
    void unlock();

    JtvLiveChannel *p_live_channel;

    QLabel *lab_channel, *lab_password, *lab_stream, /**lab_infos*/;
    QLineEdit *lne_channel, *lne_password;
    QSquareIconResizingPushButton *btn_search;
    QPushButton *btn_favourite, *btn_record, *btn_watch, *btn_advanced;
    QFrame *fra_separator;
    QComboBox *cbo_selector;
    QHBoxLayout *layout_actions;
    QGridLayout *layout;
};

#endif // JTVLIVEUITABBASICJUSTIN_TV_H
