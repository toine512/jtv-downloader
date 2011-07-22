#include "JtvLiveUiMain.h"
#include <QDesktopWidget>
#include <QtCore/Qt>

JtvLiveUiMain::JtvLiveUiMain(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Justin.tv live downloader");
    resize(425, 240);
    //Center on the current screen
    QDesktopWidget desktop_widget;
    QRect screen_geometry = desktop_widget.availableGeometry();
    move(screen_geometry.width() / 2 - width() / 2, screen_geometry.height() / 2 - height() / 2);

    //Statusbar
    ui_bottom_statusBar = statusBar();
    ui_bottom_statusBar->showMessage("I'm an useless status bar !");

    //Central zone
    ui_central_widget = new QTabWidget;

        //Page 0 : Justin.tv
        ui_central_page0 = new QWidget;
        ui_central_page0_chanName = new QLabel("Channel name :");
        ui_central_page0_chanName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_central_page0_channel = new QLineEdit;
        ui_central_page0_searchBtn = new QPushButton;
        ui_central_page0_parsingInfos = new QLabel("First, write the channel name in the field above.");
        ui_central_page0_parsingInfos->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui_central_page0_hSeparator = new QFrame;
        ui_central_page0_hSeparator->setFrameShape(QFrame::HLine);
        ui_central_page0_hSeparator->setFrameShadow(QFrame::Sunken);
        ui_central_page0_streamSelector = new QComboBox;
        ui_central_page0_streamSelector->setEnabled(false);
        ui_central_page0_bitrate = new QLabel("bitrate : n/a");
        ui_central_page0_bitrate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_viewers = new QLabel("viewers : n/a");
        ui_central_page0_viewers->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_part = new QLabel("part : n/a");
        ui_central_page0_part->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_id = new QLabel("id : n/a");
        ui_central_page0_id->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui_central_page0_node = new QLabel("node : n/a");
        ui_central_page0_node->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        //Layouts
        ui_central_page0_searchLayout = new QHBoxLayout;
        ui_central_page0_searchLayout->addWidget(ui_central_page0_chanName);
        ui_central_page0_searchLayout->addWidget(ui_central_page0_channel);
        ui_central_page0_searchLayout->addWidget(ui_central_page0_searchBtn);
        ui_central_page0_layout = new QGridLayout;
        ui_central_page0_layout->addLayout(ui_central_page0_searchLayout, 0, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_parsingInfos, 1, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_hSeparator, 2, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_streamSelector, 3, 0, 1, 3);
        ui_central_page0_layout->addWidget(ui_central_page0_bitrate, 4, 0);
        ui_central_page0_layout->addWidget(ui_central_page0_viewers, 4, 1);
        ui_central_page0_layout->addWidget(ui_central_page0_part, 4, 2);
        ui_central_page0_layout->addWidget(ui_central_page0_id, 5, 0);
        ui_central_page0_layout->addWidget(ui_central_page0_node, 5, 1);
        ui_central_page0->setLayout(ui_central_page0_layout);

    //QTabWidget setup
    ui_central_widget->addTab(ui_central_page0, "Justin.tv");

    setCentralWidget(ui_central_widget);
}

JtvLiveUiMain::~JtvLiveUiMain()
{

}
