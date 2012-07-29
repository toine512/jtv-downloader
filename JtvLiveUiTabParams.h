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

#ifndef JTVLIVEUITABPARAMS_H
#define JTVLIVEUITABPARAMS_H

#include <QString>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QPlainTextEdit>
#include <QGridLayout>

class JtvLiveUiTabParams : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabParams(JtvLiveChannel *live_channel, QWidget *parent = 0);
    
public slots:
    void clearParams();
    
protected slots:
    void setParams();
    void updateCli();

protected:
    JtvLiveChannel *p_live_channel;

    QLabel *lab_rtmp, *lab_swf, *lab_swf_vfy, *lab_web, *lab_usher_token, *lab_cli;
    QLineEdit *lne_rtmp, *lne_swf, *lne_swf_vfy, *lne_web, *lne_usher_token;
    QFrame *fra_separator;
    QPlainTextEdit *pte_cli;
    QGridLayout *layout;
};

#endif // JTVLIVEUITABPARAMS_H
