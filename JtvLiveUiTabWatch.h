#ifndef JTVLIVEUITABWATCH_H
#define JTVLIVEUITABWATCH_H

#include <QSettings>
#include <QString>
#include <QProcess>
#include "JtvLiveChannel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

class JtvLiveUiTabWatch : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabWatch(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent = 0);
    
signals:
    void askBtn_watchEnable();
    void askBtn_watchDisable();
    
public slots:
    void linkedProcessesStart();

protected slots:
    void settingsSetPlayerPath(const QString &path);
    void linkedProcessesError(const QProcess::ProcessError &error);
    void linkedProcessesDisconnectTerminate();
    void rtmpgwOut();
    void playerOut();

protected:
    void linkedProcessesTerminate();

    QSettings *p_settings;
    JtvLiveChannel *p_live_channel;
    QProcess *qproc_rtmpgw, *qproc_player;
    
    QLabel *lab_player;
    QLineEdit *lne_player;
    QPushButton *btn_watch;
    QFrame *fra_separator;
    QPlainTextEdit *pte_rtmpgw, *pte_player;
    QHBoxLayout *layout_player;
    QVBoxLayout *layout;
};

#endif // JTVLIVEUITABWATCH_H
