#ifndef JTVLIVEUITABRTMPDUMP_H
#define JTVLIVEUITABRTMPDUMP_H

#include <QSettings>
#include <QProcess>
#include "JtvLiveChannel.h"

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
