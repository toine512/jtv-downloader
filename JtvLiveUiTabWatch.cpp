/* This file is part of "Jtv live downloader"
 *
 * Copyright (C) 2012-2013 toine512 <toine512@gmail.com>
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

#include "JtvLiveUiTabWatch.h"

JtvLiveUiTabWatch::JtvLiveUiTabWatch(QSettings *settings, JtvLiveChannel *live_channel, QWidget *parent) :
    QWidget(parent)
{
    p_settings = settings;
    p_live_channel = live_channel;

    //Hosted QProcesses setup
    qproc_rtmpgw = new QProcess(this);
    qproc_rtmpgw->setProcessChannelMode(QProcess::MergedChannels);
    qproc_player = new QProcess(this);
    qproc_player->setProcessChannelMode(QProcess::MergedChannels);

    lne_player = new QLineEdit;
#ifdef Q_OS_WIN
    lab_player = new QLabel("Player path :");
    //Get the VLC default path in the Windows registry
    QSettings qs_registry("HKEY_LOCAL_MACHINE\\SOFTWARE\\VideoLAN\\VLC", QSettings::NativeFormat);
    lne_player->setText(p_settings->value("watch/player", qs_registry.value("Default").toString()).toString());
#else
    lab_player = new QLabel("Player command :");
    lne_player->setText(p_settings->value("watch/player", "vlc").toString());
#endif
    btn_player = new QPushButton;
    btn_player->setIcon(QIcon(":img/explorer.png"));
    btn_player->setToolTip("Browse...");

    btn_watch = new QPushButton("Watch");
    btn_watch->setIcon(QIcon(":img/television.png"));

    fra_separator = new QFrame;
    fra_separator->setFrameShape(QFrame::HLine);
    fra_separator->setFrameShadow(QFrame::Sunken);

    pte_rtmpgw = new QPlainTextEdit;
    pte_rtmpgw->setReadOnly(true);
    pte_rtmpgw->setUndoRedoEnabled(false);

    pte_player = new QPlainTextEdit;
    pte_player->setReadOnly(true);
    pte_rtmpgw->setUndoRedoEnabled(false);

    //Layouts
    layout_player = new QHBoxLayout;
    layout_player->addWidget(lab_player);
    layout_player->addWidget(lne_player);
    layout_player->addWidget(btn_player);

    layout = new QVBoxLayout;
    layout->addLayout(layout_player);
    layout->addWidget(btn_watch);
    layout->addWidget(fra_separator);
    layout->addWidget(pte_rtmpgw);
    layout->addWidget(pte_player);

    setLayout(layout);

    connect(lne_player, SIGNAL(textChanged(const QString &)), this, SLOT(settingsSetPlayerPath(const QString &)));
    connect(btn_player, SIGNAL(clicked()), this, SLOT(openFileBrowser()));
    connect(btn_watch, SIGNAL(clicked()), this, SLOT(linkedProcessesStart()));
    connect(qproc_rtmpgw, SIGNAL(readyReadStandardOutput()), this, SLOT(rtmpgwOut()));
    connect(qproc_player, SIGNAL(readyReadStandardOutput()), this, SLOT(playerOut()));
}

//Tab 3 slots
void JtvLiveUiTabWatch::openFileBrowser()
{
#ifdef Q_OS_WIN
    QString player = QFileDialog::getOpenFileName(this, "Choose player", QString(), "Executable (*.exe)");
#else
    QString player = QFileDialog::getOpenFileName(this, "Choose player");
#endif
    if(!player.isEmpty())
    {
        lne_player->setText(player);
    }
}

void JtvLiveUiTabWatch::settingsSetPlayerPath(const QString &path)
{
    p_settings->setValue("watch/player", path);
}

void JtvLiveUiTabWatch::linkedProcessesStart()
{

    if(lne_player->text().isEmpty())
    {
        QMessageBox::warning(this, "Player", "No player path/command provided.");
    }
    else
    {
        if(p_live_channel->isReady())
        {
            emit askBtn_watchDisable();
            btn_watch->setDisabled(true);
            pte_rtmpgw->clear();
            pte_player->clear();
            QStringList args = p_live_channel->getRtmpParams();
            args << "-g";
            args << p_settings->value("watch/port", "21080").toString();
            args << "-f";
            args << p_settings->value("flash/version", "WIN 11,1,102,62").toString();
            connect(qproc_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
            connect(qproc_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
            connect(qproc_rtmpgw, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
            connect(qproc_player, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
#ifdef Q_OS_WIN
            qproc_rtmpgw->start(p_settings->value("watch/rtmpgw", "rtmpgw.exe").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#else
            qproc_rtmpgw->start(p_settings->value("watch/rtmpgw", "rtmpgw").toString(), args, QIODevice::ReadOnly | QIODevice::Unbuffered);
#endif
            qproc_player->start(lne_player->text(), QStringList(QString("http://127.0.0.1:").append(p_settings->value("watch/port", "21080").toString())), QIODevice::ReadOnly | QIODevice::Unbuffered);

        }
        else
        {
            QMessageBox::warning(this, "Parameters", "No stream ready yet.");
        }
    }
}

void JtvLiveUiTabWatch::linkedProcessesError(const QProcess::ProcessError &error)
{
    disconnect(qproc_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(qproc_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(qproc_rtmpgw, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
    disconnect(qproc_player, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
    if(error == QProcess::FailedToStart)
    {
        QMessageBox::warning(this, "Startig process", "Unable to start the process, check rtmpgw & player path and your permissions.");
    }
    else if(error == QProcess::Crashed)
    {
        QMessageBox::critical(this, "Process crash", "The process has crashed.");
    }
    else if(error == QProcess::ReadError)
    {
        QMessageBox::critical(this, "Process read error", "Unable to read process output.");
    }
    else
    {
        QMessageBox::critical(this, "Process error", "An error has occured.");
    }
    linkedProcessesTerminate();
}

void JtvLiveUiTabWatch::linkedProcessesDisconnectTerminate()
{
    disconnect(qproc_rtmpgw, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(qproc_player, SIGNAL(error(const QProcess::ProcessError &)), this, SLOT(linkedProcessesError(const QProcess::ProcessError &)));
    disconnect(qproc_rtmpgw, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
    disconnect(qproc_player, SIGNAL(finished(int)), this, SLOT(linkedProcessesDisconnectTerminate()));
    linkedProcessesTerminate();
}

void JtvLiveUiTabWatch::rtmpgwOut()
{
    QByteArray out = qproc_rtmpgw->readAll();
    int line = out.size() - out.lastIndexOf('\r');
    if(line < 3) //We have CR LF, empty line or something that we won't care for at the end of line
    {
        pte_rtmpgw->appendPlainText(out);
    }
    else //Here we get the last line and overwrite the last one in the QPlainTextEdit
    {
        pte_rtmpgw->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        pte_rtmpgw->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        pte_rtmpgw->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
        pte_rtmpgw->textCursor().removeSelectedText();
        pte_rtmpgw->insertPlainText(out.right(line - 1));
    }
}

void JtvLiveUiTabWatch::playerOut()
{
    pte_player->appendPlainText(qproc_player->readAll());
}

void JtvLiveUiTabWatch::linkedProcessesTerminate()
{
#ifdef Q_OS_WIN
    qproc_rtmpgw->kill();
#else
    qproc_rtmpgw->terminate();
#endif
    qproc_player->terminate();
    emit askBtn_watchEnable();
    btn_watch->setEnabled(true);
}
