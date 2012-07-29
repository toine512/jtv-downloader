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

#ifndef JTVLIVEUITABRTMPGW_H
#define JTVLIVEUITABRTMPGW_H

#include <QtGlobal>
#include <QString>
#include <QSettings>
#include <QProcess>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class JtvLiveUiTabRtmpgw : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabRtmpgw(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent = 0);
    
protected slots:
    void settingsSetIp(const QString &ip);
    void settingsSetPort(int port);
    void startRtmpgw();

protected:
    QSettings *p_settings;
    JtvLiveChannel *p_live_channel;

    QGroupBox *group_parameters, *group_verbosity;
    QGridLayout *layout_parameters;
    QLineEdit *lne_ip;
    QSpinBox *spb_port;
    QLabel *lab_ip, *lab_port, *lab_colon;
    QHBoxLayout *layout_verbosity;
    QRadioButton *rbtn_verbosity_normal, *rbtn_verbosity_verbose, *rbtn_verbosity_debug;
    QPushButton *btn_start;
    QVBoxLayout *layout;
    
};

#endif // JTVLIVEUITABRTMPGW_H
