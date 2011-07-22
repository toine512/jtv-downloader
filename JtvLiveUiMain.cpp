#include "JtvLiveUiMain.h"
#include <QDesktopWidget>
#include <QtCore/Qt>
#include <QIcon>

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
        ui_central_page0_searchBtn->setIcon(QIcon(":img/zoom.png"));
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

        //Page 1 : Parameters
        ui_central_page1 = new QWidget;
        ui_central_page1_l_rtmp = new QLabel("RTMP (-r)");
        ui_central_page1_rtmp = new QLineEdit;
        ui_central_page1_l_swf = new QLabel("SWF (-s)");
        ui_central_page1_swf = new QLineEdit;
        ui_central_page1_l_swfVfy = new QLabel("SWF verif (-W)");
        ui_central_page1_swfVfy = new QLineEdit;
        ui_central_page1_l_web = new QLabel("Web page (-p)");
        ui_central_page1_web = new QLineEdit;
        ui_central_page1_l_usherToken = new QLabel("UsherToken (-j)");
        ui_central_page1_usherToken = new QTextEdit;
        //Layout
        ui_central_page1_layout = new QGridLayout;
        ui_central_page1_layout->addWidget(ui_central_page1_l_rtmp, 0, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_rtmp, 0, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_swf, 1, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_swf, 1, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_swfVfy, 2, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_swfVfy, 2, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_web, 3, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_web, 3, 1);
        ui_central_page1_layout->addWidget(ui_central_page1_l_usherToken, 4, 0);
        ui_central_page1_layout->addWidget(ui_central_page1_usherToken, 4, 1);
        ui_central_page1->setLayout(ui_central_page1_layout);

        //Page 2 : rtmpdump
        ui_central_page2 = new QWidget;
        ui_central_page2_file_box = new QGroupBox("Outfile :");
        ui_central_page2_file_box->setCheckable(true);
        ui_central_page2_file_box->setChecked(true);
        ui_central_page2_file = new QLineEdit;
        ui_central_page2_file_btn = new QPushButton("...");
        ui_central_page2_file_layout = new QHBoxLayout();
        ui_central_page2_file_layout->addWidget(ui_central_page2_file);
        ui_central_page2_file_layout->addWidget(ui_central_page2_file_btn);
        ui_central_page2_file_box->setLayout(ui_central_page2_file_layout);
        ui_central_page2_pipe_box = new QGroupBox("Pipe :");
        ui_central_page2_pipe_box->setCheckable(true);
        ui_central_page2_pipe_box->setChecked(false);
        ui_central_page2_pipe = new QLineEdit;
        ui_central_page2_pipe_layout = new QHBoxLayout;
        ui_central_page2_pipe_layout->addWidget(ui_central_page2_pipe);
        ui_central_page2_pipe_box->setLayout(ui_central_page2_pipe_layout);
        ui_central_page2_verbosity_box = new QGroupBox("Verbosity :");
        ui_central_page2_verbosity_normal = new QRadioButton("Normal");
        ui_central_page2_verbosity_normal->setChecked(true);
        ui_central_page2_verbosity_verbose = new QRadioButton("Verbose");
        ui_central_page2_verbosity_debug = new QRadioButton("Debug");
        ui_central_page2_verbosity_layout = new QHBoxLayout;
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_normal);
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_verbose);
        ui_central_page2_verbosity_layout->addWidget(ui_central_page2_verbosity_debug);
        ui_central_page2_verbosity_box->setLayout(ui_central_page2_verbosity_layout);
        ui_central_page2_start = new QPushButton("Start");
        //Layout
        ui_central_page2_layout = new QVBoxLayout;
        ui_central_page2_layout->addWidget(ui_central_page2_file_box);
        ui_central_page2_layout->addWidget(ui_central_page2_pipe_box);
        ui_central_page2_layout->addWidget(ui_central_page2_verbosity_box);
        ui_central_page2_layout->addWidget(ui_central_page2_start);
        ui_central_page2->setLayout(ui_central_page2_layout);


    //QTabWidget setup
    ui_central_widget->addTab(ui_central_page0, "Justin.tv");
    ui_central_widget->addTab(ui_central_page1, "Params");
    ui_central_widget->addTab(ui_central_page2, "rtmpdump");

    //Central signals/slots
    connect(ui_central_page2_pipe_box, SIGNAL(toggled(bool)), this, SLOT(Page2_toggleFileCheck(bool)));
    connect(ui_central_page2_file_box, SIGNAL(toggled(bool)), this, SLOT(Page2_togglePipeCheck(bool)));

    setCentralWidget(ui_central_widget);
}

//Called by ui_central_page2_pipe_box
void JtvLiveUiMain::Page2_toggleFileCheck(bool pipe_ckecked)
{
    if(pipe_ckecked)
    {
        ui_central_page2_file_box->setChecked(false);
    }
    else
    {
        ui_central_page2_file_box->setChecked(true);
    }
}

//Called by ui_central_page2_file_box
void JtvLiveUiMain::Page2_togglePipeCheck(bool file_ckecked)
{
    if(file_ckecked)
    {
        ui_central_page2_pipe_box->setChecked(false);
    }
    else
    {
        ui_central_page2_pipe_box->setChecked(true);
    }
}

JtvLiveUiMain::~JtvLiveUiMain()
{

}
