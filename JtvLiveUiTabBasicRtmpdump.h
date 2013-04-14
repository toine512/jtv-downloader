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

#ifndef JTVLIVEUITABBASICRTMPDUMP_H
#define JTVLIVEUITABBASICRTMPDUMP_H

#include <QtGlobal>
#include <QString>
#include <QSettings>
#include <QProcess>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>

class JtvLiveUiTabBasicRtmpdump : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabBasicRtmpdump(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent = 0);

protected slots:
    void openFileBrowser();
    void startRtmpdump();

protected:
    QSettings *p_settings;
    JtvLiveChannel *p_live_channel;

    QHBoxLayout *layout_file;
    QLabel *lab_file, *lab_tip;
    QLineEdit *lne_file;
    QPushButton *btn_file, *btn_start;
    QVBoxLayout *layout;
};

#endif // JTVLIVEUITABBASICRTMPDUMP_H
