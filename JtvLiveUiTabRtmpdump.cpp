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
 * along with "Jtv live downloader".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JtvLiveUiTabRtmpdump.h"

JtvLiveUiTabRtmpdump::JtvLiveUiTabRtmpdump(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_settings = settings;
    p_live_channel = live_channel;

    group_file = new QGroupBox("Outfile :");
    group_file->setCheckable(true);
    group_file->setChecked(true);

    lne_file = new QLineEdit;
    btn_file = new QPushButton;
    btn_file->setIcon(QIcon(":img/explorer.png"));
    btn_file->setToolTip("Browse...");

    layout_file = new QHBoxLayout();
    layout_file->addWidget(lne_file);
    layout_file->addWidget(btn_file);

    group_file->setLayout(layout_file);

    group_pipe = new QGroupBox("Pipe :");
    group_pipe->setCheckable(true);
    group_pipe->setChecked(false);

    lne_pipe = new QLineEdit;
    lne_pipe->setToolTip("Right side of the pipe (after | )");
    lne_pipe->setText(p_settings->value("rtmpdump/pipe").toString());

    layout_pipe = new QHBoxLayout;
    layout_pipe->addWidget(lne_pipe);

    group_pipe->setLayout(layout_pipe);

    group_verbosity = new QGroupBox("Verbosity :");

    rbtn_verbosity_normal = new QRadioButton("Normal");
    rbtn_verbosity_normal->setChecked(true);

    rbtn_verbosity_verbose = new QRadioButton("Verbose");

    rbtn_verbosity_debug = new QRadioButton("Debug");

    layout_verbosity = new QHBoxLayout;
    layout_verbosity->addWidget(rbtn_verbosity_normal);
    layout_verbosity->addWidget(rbtn_verbosity_verbose);
    layout_verbosity->addWidget(rbtn_verbosity_debug);

    group_verbosity->setLayout(layout_verbosity);

    btn_start = new QPushButton(QIcon(":img/player.png"), "Start");

    //Layout
    layout = new QVBoxLayout;
    layout->addWidget(group_file);
    layout->addWidget(group_pipe);
    layout->addWidget(group_verbosity);
    layout->addWidget(btn_start);

    setLayout(layout);

    connect(btn_file, SIGNAL(clicked()), this, SLOT(openFileBrowser()));
    connect(group_pipe, SIGNAL(toggled(bool)), this, SLOT(toggleFileCheck(bool)));
    connect(group_file, SIGNAL(toggled(bool)), this, SLOT(togglePipeCheck(bool)));
    connect(lne_pipe, SIGNAL(textEdited(const QString &)), this, SLOT(settingsSetPipe(const QString &)));
    connect(btn_start, SIGNAL(clicked()), this, SLOT(startRtmpdump()));
}

void JtvLiveUiTabRtmpdump::openFileBrowser()
{
    lne_file->setText(QFileDialog::getSaveFileName(this, "Save stream", QString(), "Flash video (*.flv)"));
}

//Called by group_pipe
void JtvLiveUiTabRtmpdump::toggleFileCheck(bool pipe_ckecked)
{
    if(pipe_ckecked)
    {
        group_file->setChecked(false);
    }
    else
    {
        group_file->setChecked(true);
    }
}

//Called by group_file
void JtvLiveUiTabRtmpdump::togglePipeCheck(bool file_ckecked)
{
    if(file_ckecked)
    {
        group_pipe->setChecked(false);
    }
    else
    {
        group_pipe->setChecked(true);
    }
}

void JtvLiveUiTabRtmpdump::settingsSetPipe(const QString &text)
{
    p_settings->setValue("rtmpdump/pipe", text);
}

void JtvLiveUiTabRtmpdump::startRtmpdump()
{
    if(p_live_channel->isReady())
    {
        QStringList args = p_live_channel->getRtmpParams();
        args << "-f";
        args << p_settings->value("flash/version", "WIN 11,1,102,62").toString();

        if(group_file->isChecked())
        {
            if(lne_file->text().isEmpty())
            {
                QMessageBox::warning(this, "Output file", "No output path provided.");
            }
            else
            {
                if(rbtn_verbosity_verbose->isChecked())
                {
                    args << "-V";
                }
                else if(rbtn_verbosity_debug->isChecked())
                {
                    args << "-z";
                }
                args << "-o";
                args << lne_file->text();
#ifdef Q_OS_WIN
                if(!QProcess::startDetached(p_settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), args))
#else
                if(!QProcess::startDetached(p_settings->value("terminal/terminal", "xterm").toString(), QStringList() << p_settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2").arg(p_settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), JtvLiveChannel::escape4CLI(args))))
#endif
                {
                    QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
                }
            }
        }
        else if(group_pipe->isChecked())
        {
            if(lne_pipe->text().isEmpty())
            {
                QMessageBox::warning(this, "Piping", "Right side of the pipe is empty.");
            }
            else
            {
                args << "-q";
#ifdef Q_OS_WIN
                if(!QProcess::startDetached(QString("cmd.exe /c \"%1 %2 | %3\"").arg(p_settings->value("rtmpdump/rtmpdump", "rtmpdump.exe").toString(), JtvLiveChannel::escape4CLI(args).replace("\\\"", "\"\"\""), lne_pipe->text()))) //epic Windows crap ! -> replace("\\\"", "\"\"\"")
#else
                if(!QProcess::startDetached(p_settings->value("terminal/terminal", "xterm").toString(), QStringList() << p_settings->value("terminal/cmdswitch", "-e").toString() << QString("%1 %2 | %3").arg(p_settings->value("rtmpdump/rtmpdump", "rtmpdump").toString(), JtvLiveChannel::escape4CLI(args), lne_pipe->text())))
#endif
                {
                    QMessageBox::warning(this, "Launching rtmpdump", "Unable to create the process, check the path.");
                }
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Parameters", "No stream ready yet.");
    }
}
