#ifndef JTVLIVEUITABPARAMS_H
#define JTVLIVEUITABPARAMS_H

#include "JtvLiveChannel.h"

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
