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

#include "JtvLiveUiTabAbout.h"

JtvLiveUiTabAbout::JtvLiveUiTabAbout(QWidget *parent) :
    QWidget(parent)
{
    lab_notice = new QLabel(QString("<p align=\"center\"><b>Jtv live downloader v. %1 %2</b><br />Copyright © 2012 toine512<br />Compiled on [%3] [%4]<br /><br />This software is distributed under <a href=\"https://www.gnu.org/licenses/gpl.html\">GNU General Public License v. 3</a>.</p><p><a href=\"http://stream-recorder.com/forum/justin-tv-live-downloader-t11128.html\">Forum thread</a> on stream-recorder.com</p><p>Written in C++ using <a href=\"https://qt.nokia.com/\">Qt</a> 4.7.4 (<a href=\"https://www.gnu.org/licenses/gpl.html\">GNU GPL v. 3</a>).<br />Uses <a href=\"http://www.famfamfam.com/lab/icons/silk/\">FAMFAMFAM Silk Icons</a> by <a href=\"http://www.famfamfam.com/\">Mark James</a> (<a href=\"https://creativecommons.org/licenses/by/2.5/\">CC BY 2.5</a>).</p>").arg(JTV_LIVE_VERSION_HUMAN, UpdateChecker::whichRelease(JTV_LIVE_VERSION), __DATE__, __TIME__));
    lab_notice->setOpenExternalLinks(true);

    lab_gplv3 = new QLabel;
    lab_gplv3->setPixmap(QPixmap(":img/gplv3.png"));

    btn_aboutQt = new QPushButton("About Qt");

    //Layout
    layout = new QGridLayout;
    layout->addWidget(lab_notice, 0, 0, 1, 2);
    layout->addWidget(lab_gplv3, 1, 0, 1, 1);
    layout->addWidget(btn_aboutQt, 1, 1, 1, 1);

    setLayout(layout);

    connect(btn_aboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
}

void JtvLiveUiTabAbout::aboutQt()
{
    QMessageBox::aboutQt(this);
}
