/* Copyright (C) 2012 toine512 <toine512@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QSIRPUSHBUTTON_H
#define QSIRPUSHBUTTON_H

#include "QSquarePushButton.h"
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>

class QSquareIconResizingPushButton : public QSquarePushButton
{
    Q_OBJECT

public:
    explicit QSquareIconResizingPushButton(const QString &pixmap_file, QWidget *parent = 0);

protected:
    QLabel *container;    
};

#endif // QSIRPUSHBUTTON_H
