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
 * along with "TV sur PC Desktop".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JTVLIVEUIMAIN_H
#define JTVLIVEUIMAIN_H

#include "defines.h"

#include <QtCore/Qt>
#include <QSettings>
#include <QStringList>
#include <QProcess>

#include <QNetworkAccessManager>

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
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

#include "JtvLiveChannel.h"

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    QString getCommandEscaped(QStringList args);
    ~JtvLiveUiMain();

protected slots:
    void Page0_searchChannel();
    void Page0_onMessageChanged(const QString &message);
    void Page0_onSearchSuccess(QList<JtvLiveStream> *streams);
    void Page0_onSearchError(const QString &error);
    void Page0_updateStreamDatas(int index);
    void Page0_gotoWatchAndStart();
    void Page1_buildCliFriendly();
    void Page2_browseFile();
    void Page2_toggleFileCheck(bool pipe_ckecked);
    void Page2_togglePipeCheck(bool file_ckecked);
    void Page2_savePipe(const QString &text);
    void Page2_startRtmpdump();
    void Page3_savePlayerPath(const QString &path);
    void Page3_linkedProcessesStart();
    void Page3_linkedProcessesError(const QProcess::ProcessError &error);
    void Page3_linkedProcessesDisconnectTerminate();
    void Page3_rtmpgwOut();
    void Page3_playerOut();
    void Page4_saveIp(const QString &ip);
    void Page4_savePort(int port);
    void Page4_startRtmpgw();
    void aboutQt();

protected:
    void Page0_lock();
    void Page0_unlock();
    void Page0_defaultStats();
    void Page0_fillStats(const JtvLiveStream &stream);
    void Page1_defaultParams();
    void Page1_fillParams(const JtvLiveStream &stream);
    void Page2_startRtmpdumpFile(const QString &path);
    void Page3_linkedProcessesTerminate();
    QStringList collectRtmpParams();

    //UI
    //QStatusBar *ui_bottom_statusBar;
    QTabWidget *ui_widget;
    QWidget *ui_page0, *ui_page1, *ui_page2, *ui_page3, *ui_page4, *ui_page5;

    //Page 0 : Justin.tv
    QLabel *ui_page0_chanName, *ui_page0_parsingInfos, *ui_page0_bitrate, *ui_page0_viewers, *ui_page0_part, *ui_page0_id, *ui_page0_node;
    QLineEdit *ui_page0_channel;
    QPushButton *ui_page0_searchBtn, *ui_page0_gotoWatch;
    QFrame *ui_page0_hSeparator;
    QComboBox *ui_page0_streamSelector;
    QHBoxLayout *ui_page0_searchLayout, *ui_page0_streamLayout;
    QGridLayout *ui_page0_layout;

    //Page 1 : Parameters
    QLabel *ui_page1_l_rtmp, *ui_page1_l_swf, *ui_page1_l_swfVfy, *ui_page1_l_web, *ui_page1_l_usherToken, *ui_page1_l_cliFriendly;
    QLineEdit *ui_page1_rtmp, *ui_page1_swf, *ui_page1_swfVfy, *ui_page1_web, *ui_page1_usherToken;
    QFrame *ui_page1_hSeparator;
    QPlainTextEdit *ui_page1_cliFriendly;
    QGridLayout *ui_page1_layout;

    //Page 2 : rtmpdump
    QGroupBox *ui_page2_file_box, *ui_page2_pipe_box, *ui_page2_verbosity_box;
    QHBoxLayout *ui_page2_file_layout, *ui_page2_pipe_layout, *ui_page2_verbosity_layout;
    QLineEdit *ui_page2_file, *ui_page2_pipe;
    QPushButton *ui_page2_file_btn, *ui_page2_start;
    QRadioButton *ui_page2_verbosity_normal, *ui_page2_verbosity_verbose, *ui_page2_verbosity_debug;
    QVBoxLayout *ui_page2_layout;

    //Page 3 : Play
    QHBoxLayout *ui_page3_player_layout;
    QLabel *ui_page3_player_label;
    QLineEdit *ui_page3_player;
    QPushButton *ui_page3_watchBtn;
    QFrame *ui_page3_hSeparator;
    QPlainTextEdit *ui_page3_rtmpgwOut, *ui_page3_playerOut;
    QVBoxLayout *ui_page3_layout;

    //Page 4 : rtmpdump
    QGroupBox *ui_page4_params_box, *ui_page4_verbosity_box;
    QGridLayout *ui_page4_params_layout;
    QLineEdit *ui_page4_params_ip;
    QSpinBox *ui_page4_params_port;
    QLabel *ui_page4_params_l_ip, *ui_page4_params_l_port, *ui_page4_params_colon;
    QHBoxLayout *ui_page4_verbosity_layout;
    QRadioButton *ui_page4_verbosity_normal, *ui_page4_verbosity_verbose, *ui_page4_verbosity_debug;
    QPushButton *ui_page4_start;
    QVBoxLayout *ui_page4_layout;

    //Page 5 : About
    QLabel *ui_page5_copyrightNotice, *ui_page5_gplv3;
    QPushButton *ui_page5_aboutQt;
    QGridLayout *ui_page5_layout;

    //Core
    QSettings *settings;
    QNetworkAccessManager *net_manager;
    JtvLiveChannel *live_channel;
    QProcess *linkedProcess_rtmpgw;
    QProcess *linkedProcess_player;
};

#endif // JTVLIVEUIMAIN_H
