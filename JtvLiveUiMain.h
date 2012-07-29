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

#ifndef JTVLIVEUIMAIN_H
#define JTVLIVEUIMAIN_H

#include "defines.h"

#include <QtCore/Qt>
#include <QSettings>
#include <QUuid>
#include <QStringList>
#include <QProcess>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QNetworkAccessManager>
#include "UpdateChecker.h"

//#include <QStatusBar>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QFrame>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include "JtvLiveUiTabJustin_tv.h"
#include "JtvLiveUiTabWatch.h"
#include "JtvLiveUiTabParams.h"
#include "JtvLiveUiTabRtmpdump.h"
#include "JtvLiveUiTabRtmpgw.h"

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    ~JtvLiveUiMain();

protected slots:
    void onGotoWatchAndStart();




    void TabUpdate_show(const QString &new_version_human, const QString &dl_link);
    void aboutQt();

protected:






    //UI
    //QStatusBar *ui_bottom_statusBar;
    QTabWidget *ui_widget;
    QWidget *ui_tab5, *ui_tabUpdate;

    JtvLiveUiTabJustin_tv *ui_tab0;
    JtvLiveUiTabParams *ui_tab1;
    JtvLiveUiTabWatch *ui_tab3;
    JtvLiveUiTabRtmpdump *ui_tab2;
    JtvLiveUiTabRtmpgw *ui_tab4;

    //Tab 2 : rtmpdump




    //Tab 4 : rtmpdump


    //Tab 5 : About
    QLabel *ui_tab5_copyrightNotice, *ui_tab5_gplv3;
    QPushButton *ui_tab5_aboutQt;
    QGridLayout *ui_tab5_layout;

    //Update Tab
    QLabel *ui_tabUpdate_notice;
    QPlainTextEdit *ui_tabUpdate_notes;
    QVBoxLayout *ui_tabUpdate_layout;

    //Core
    QSettings *settings;
    QNetworkAccessManager *net_manager;
    JtvLiveChannel *live_channel;
    UpdateChecker *updater;

};

#endif // JTVLIVEUIMAIN_H
