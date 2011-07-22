#ifndef JTVLIVEUIMAIN_H
#define JTVLIVEUIMAIN_H

#include <QtGui/QMainWindow>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>

class JtvLiveUiMain : public QMainWindow
{
    Q_OBJECT

public:
    JtvLiveUiMain(QWidget *parent = 0);
    ~JtvLiveUiMain();

signals:

public slots:

private:
    QStatusBar *ui_bottom_statusBar;

    QTabWidget *ui_central_widget;
    QWidget *ui_central_page0, *ui_central_page1, *ui_central_page2, *ui_central_page3, *ui_central_page4;

    QLabel *ui_central_page0_chanName;
    QLineEdit *ui_central_page0_channel;
    QPushButton *ui_central_page0_searchBtn;
    QLabel *ui_central_page0_parsingInfos;
    QFrame *ui_central_page0_hSeparator;
    QComboBox *ui_central_page0_streamSelector;
    QLabel *ui_central_page0_bitrate, *ui_central_page0_viewers, *ui_central_page0_part, *ui_central_page0_id, *ui_central_page0_node;
    QHBoxLayout *ui_central_page0_searchLayout;
    QGridLayout *ui_central_page0_layout;

};

#endif // JTVLIVEUIMAIN_H
