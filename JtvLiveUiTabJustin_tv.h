#ifndef JTVLIVEUITABJUSTIN_TV_H
#define JTVLIVEUITABJUSTIN_TV_H

#include <QString>
#include <QStringList>
#include "qt-jtvlive/JtvLiveChannel.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "QSIRPushButton.h"

class JtvLiveUiTabJustin_tv : public QWidget
{
    Q_OBJECT

public:
    explicit JtvLiveUiTabJustin_tv(JtvLiveChannel *live_channel, QWidget *parent = 0);
    
signals:
    void gotoWatchAndStart();
    void askClearParams();

public slots:
    void btn_watchEnable();
    void btn_watchDisable();

protected slots:
    void searchChannel();
    void onMessageChanged(const QString &message);
    void onSearchSuccess(QStringList names);
    void onSearchError(const QString &error);
    void fillStats();


protected:
    void lock();
    void unlock();
    void defaultStats();

    JtvLiveChannel *p_live_channel;

    QLabel *lab_channel, *lab_password, *lab_infos, *lab_bitrate, *lab_viewers, *lab_part, *lab_id, *lab_node;
    QLineEdit *lne_channel, *lne_password;
    QPushButton *btn_favourite, *btn_search, *btn_watch;
    QFrame *fra_separator;
    QComboBox *cbo_selector;
    QHBoxLayout *layout_stream;
    QGridLayout *layout_search, *layout_stats;
    QVBoxLayout *layout;
    
};

#endif // JTVLIVEUITABJUSTIN_TV_H
