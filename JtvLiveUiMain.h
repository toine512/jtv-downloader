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

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    QString getCommandEscaped(QStringList args);
    ~JtvLiveUiMain();

protected slots:
    void onGotoWatchAndStart();



    void Tab4_saveIp(const QString &ip);
    void Tab4_savePort(int port);
    void Tab4_startRtmpgw();
    void TabUpdate_show(const QString &new_version_human, const QString &dl_link);
    void aboutQt();

protected:




    QStringList collectRtmpParams();

    //UI
    //QStatusBar *ui_bottom_statusBar;
    QTabWidget *ui_widget;
    QWidget *ui_tab4, *ui_tab5, *ui_tabUpdate;

    JtvLiveUiTabJustin_tv *ui_tab0;
    JtvLiveUiTabParams *ui_tab1;
    JtvLiveUiTabWatch *ui_tab3;
    JtvLiveUiTabRtmpdump *ui_tab2;

    //Tab 2 : rtmpdump




    //Tab 4 : rtmpdump
    QGroupBox *ui_tab4_params_box, *ui_tab4_verbosity_box;
    QGridLayout *ui_tab4_params_layout;
    QLineEdit *ui_tab4_params_ip;
    QSpinBox *ui_tab4_params_port;
    QLabel *ui_tab4_params_l_ip, *ui_tab4_params_l_port, *ui_tab4_params_colon;
    QHBoxLayout *ui_tab4_verbosity_layout;
    QRadioButton *ui_tab4_verbosity_normal, *ui_tab4_verbosity_verbose, *ui_tab4_verbosity_debug;
    QPushButton *ui_tab4_start;
    QVBoxLayout *ui_tab4_layout;

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
