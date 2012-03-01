#ifndef JTVLIVEUIMAIN_H
#define JTVLIVEUIMAIN_H

#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QStringList>
#include <QSettings>
#include "JtvLiveChannel.h"

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    ~JtvLiveUiMain();

signals:

public slots:

protected slots:
    void Page0_updateStreamDatas(int index);
    void Page0_searchChannel();
    void Page0_onMessageChanged(const QString &message);
    void Page0_onSearchSuccess(QList<JtvLiveStream> *streams);
    void Page0_onSearchError(const QString &error);
    void Page1_buildCliFriendly();
    void Page2_browseFile();
    void Page2_toggleFileCheck(bool pipe_ckecked);
    void Page2_togglePipeCheck(bool file_ckecked);
    void Page3_savePlayerPath(const QString &path);
    void Page3_linkedProcessesStart();
    void Page3_linkedProcessesError(const QProcess::ProcessError &error);
    void Page3_linkedProcessesTerminate();
    void Page3_rtmpgwOut();
    void Page3_playerOut();

protected:
    QStringList collectRtmpParams();
    void Page0_lock();
    void Page0_unlock();
    void Page0_defaultStats();
    void Page0_fillStats(const JtvLiveStream &stream);
    void Page1_defaultParams();
    void Page1_fillParams(const JtvLiveStream &stream);

    //UI
    QStatusBar *ui_bottom_statusBar;

    QTabWidget *ui_central_widget;
    QWidget *ui_central_page0, *ui_central_page1, *ui_central_page2, *ui_central_page3, *ui_central_page4;

    //Page 0 : Justin.tv
    QLabel *ui_central_page0_chanName;
    QLineEdit *ui_central_page0_channel;
    QPushButton *ui_central_page0_searchBtn;
    QLabel *ui_central_page0_parsingInfos;
    QFrame *ui_central_page0_hSeparator;
    QComboBox *ui_central_page0_streamSelector;
    QLabel *ui_central_page0_bitrate, *ui_central_page0_viewers, *ui_central_page0_part, *ui_central_page0_id, *ui_central_page0_node;
    QHBoxLayout *ui_central_page0_searchLayout;
    QGridLayout *ui_central_page0_layout;

    //Page 1 : Parameters
    QLabel *ui_central_page1_l_rtmp, *ui_central_page1_l_swf, *ui_central_page1_l_swfVfy, *ui_central_page1_l_web, *ui_central_page1_l_usherToken, *ui_central_page1_l_cliFriendly;
    QLineEdit *ui_central_page1_rtmp, *ui_central_page1_swf, *ui_central_page1_swfVfy, *ui_central_page1_web, *ui_central_page1_usherToken;
    QFrame *ui_central_page1_hSeparator;
    QPlainTextEdit *ui_central_page1_cliFriendly;
    QGridLayout *ui_central_page1_layout;

    //Page 2 : rtmpdump
    QGroupBox *ui_central_page2_file_box, *ui_central_page2_pipe_box, *ui_central_page2_verbosity_box;
    QHBoxLayout *ui_central_page2_file_layout, *ui_central_page2_pipe_layout, *ui_central_page2_verbosity_layout;
    QLineEdit *ui_central_page2_file;
    QPushButton *ui_central_page2_file_btn;
    QLineEdit *ui_central_page2_pipe;
    QRadioButton *ui_central_page2_verbosity_normal, *ui_central_page2_verbosity_verbose, *ui_central_page2_verbosity_debug;
    QPushButton *ui_central_page2_start;
    QVBoxLayout *ui_central_page2_layout;

    //Page 3 : Play
    QHBoxLayout *ui_central_page3_player_layout;
    QLabel *ui_central_page3_player_label;
    QLineEdit *ui_central_page3_player;
    QPushButton *ui_central_page3_watchBtn;
    QFrame *ui_central_page3_hSeparator;
    QPlainTextEdit *ui_central_page3_rtmpgwOut, *ui_central_page3_playerOut;
    QVBoxLayout *ui_central_page3_layout;

    //Core
    QSettings *settings;
    JtvLiveChannel *live_channel;
    QProcess *linkedProcess_rtmpgw;
    QProcess *linkedProcess_player;
};

#endif // JTVLIVEUIMAIN_H
