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
#include "JtvLiveChannel.h"

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
#include "QSIRPushButton.h"

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    QString getCommandEscaped(QStringList args);
    ~JtvLiveUiMain();

protected slots:
    void Tab0_searchChannel();
    void Tab0_onMessageChanged(const QString &message);
    void Tab0_onSearchSuccess(QList<JtvLiveStream> *streams);
    void Tab0_onSearchError(const QString &error);
    void Tab0_updateStreamDatas(int index);
    void Tab0_gotoWatchAndStart();
    void Tab1_buildCliFriendly();
    void Tab2_browseFile();
    void Tab2_toggleFileCheck(bool pipe_ckecked);
    void Tab2_togglePipeCheck(bool file_ckecked);
    void Tab2_savePipe(const QString &text);
    void Tab2_startRtmpdump();
    void Tab3_savePlayerPath(const QString &path);
    void Tab3_linkedProcessesStart();
    void Tab3_linkedProcessesError(const QProcess::ProcessError &error);
    void Tab3_linkedProcessesDisconnectTerminate();
    void Tab3_rtmpgwOut();
    void Tab3_playerOut();
    void Tab4_saveIp(const QString &ip);
    void Tab4_savePort(int port);
    void Tab4_startRtmpgw();
    void TabUpdate_show(const QString &new_version_human, const QString &dl_link);
    void aboutQt();

protected:
    void Tab0_lock();
    void Tab0_unlock();
    void Tab0_defaultStats();
    void Tab0_fillStats(const JtvLiveStream &stream);
    void Tab1_defaultParams();
    void Tab1_fillParams(const JtvLiveStream &stream);
    void Tab2_startRtmpdumpFile(const QString &path);
    void Tab3_linkedProcessesTerminate();
    QStringList collectRtmpParams();

    //UI
    //QStatusBar *ui_bottom_statusBar;
    QTabWidget *ui_widget;
    QWidget *ui_tab0, *ui_tab1, *ui_tab2, *ui_tab3, *ui_tab4, *ui_tab5, *ui_tabUpdate;

    //Tab 0 : Justin.tv
    QLabel *ui_tab0_chanLabel, *ui_tab0_passwdLabel, *ui_tab0_parsingInfos, *ui_tab0_bitrate, *ui_tab0_viewers, *ui_tab0_part, *ui_tab0_id, *ui_tab0_node;
    QLineEdit *ui_tab0_channel, *ui_tab0_password;
    QPushButton *ui_tab0_favouriteBtn, *ui_tab0_searchBtn, *ui_tab0_gotoWatch;
    QFrame *ui_tab0_hSeparator;
    QComboBox *ui_tab0_streamSelector;
    QHBoxLayout *ui_tab0_streamLayout;
    QGridLayout *ui_tab0_searchLayout, *ui_tab0_statsLayout;
    QVBoxLayout *ui_tab0_layout;

    //Tab 1 : Parameters
    QLabel *ui_tab1_l_rtmp, *ui_tab1_l_swf, *ui_tab1_l_swfVfy, *ui_tab1_l_web, *ui_tab1_l_usherToken, *ui_tab1_l_cliFriendly;
    QLineEdit *ui_tab1_rtmp, *ui_tab1_swf, *ui_tab1_swfVfy, *ui_tab1_web, *ui_tab1_usherToken;
    QFrame *ui_tab1_hSeparator;
    QPlainTextEdit *ui_tab1_cliFriendly;
    QGridLayout *ui_tab1_layout;

    //Tab 2 : rtmpdump
    QGroupBox *ui_tab2_file_box, *ui_tab2_pipe_box, *ui_tab2_verbosity_box;
    QHBoxLayout *ui_tab2_file_layout, *ui_tab2_pipe_layout, *ui_tab2_verbosity_layout;
    QLineEdit *ui_tab2_file, *ui_tab2_pipe;
    QPushButton *ui_tab2_file_btn, *ui_tab2_start;
    QRadioButton *ui_tab2_verbosity_normal, *ui_tab2_verbosity_verbose, *ui_tab2_verbosity_debug;
    QVBoxLayout *ui_tab2_layout;

    //Tab 3 : Play
    QHBoxLayout *ui_tab3_player_layout;
    QLabel *ui_tab3_player_label;
    QLineEdit *ui_tab3_player;
    QPushButton *ui_tab3_watchBtn;
    QFrame *ui_tab3_hSeparator;
    QPlainTextEdit *ui_tab3_rtmpgwOut, *ui_tab3_playerOut;
    QVBoxLayout *ui_tab3_layout;

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
    QProcess *linkedProcess_rtmpgw;
    QProcess *linkedProcess_player;
};

#endif // JTVLIVEUIMAIN_H
