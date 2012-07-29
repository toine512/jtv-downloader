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

#ifndef JTVLIVEUITABRTMPDUMP_H
#define JTVLIVEUITABRTMPDUMP_H

#include <QtGlobal>
#include <QString>
#include <QSettings>
#include <QProcess>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include <QFileDialog>

class JtvLiveUiTabRtmpdump : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabRtmpdump(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent = 0);

protected slots:
    void openFileBrowser();
    void toggleFileCheck(bool pipe_ckecked);
    void togglePipeCheck(bool file_ckecked);
    void settingsSetPipe(const QString &text);
    void startRtmpdump();

protected:
    QSettings *p_settings;
    JtvLiveChannel *p_live_channel;

    QGroupBox *group_file, *group_pipe, *group_verbosity;
    QHBoxLayout *layout_file, *layout_pipe, *layout_verbosity;
    QLineEdit *lne_file, *lne_pipe;
    QPushButton *btn_file, *btn_start;
    QRadioButton *rbtn_verbosity_normal, *rbtn_verbosity_verbose, *rbtn_verbosity_debug;
    QVBoxLayout *layout;
};

#endif // JTVLIVEUITABRTMPDUMP_H
