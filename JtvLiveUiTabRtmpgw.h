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
