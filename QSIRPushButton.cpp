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

#include "QSIRPushButton.h"

QSIRPushButton::QSIRPushButton(const QString &pixmap_file, QWidget *parent) :
    QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    container = new QLabel;
    container->setPixmap(QPixmap(pixmap_file));
    container->setScaledContents(true);
    setLayout(new QHBoxLayout);
    layout()->addWidget(container);
}

QSize QSIRPushButton::sizeHint() const
{
    QSize size = QPushButton::sizeHint();
    size.setWidth(height());
    return size;
}
