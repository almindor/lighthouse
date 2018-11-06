/*
    Copyright (C) 2018 Lukáš Karas <lukas.karas@centrum.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "zram.h"

namespace Lighthouse {


Zram::Zram(QObject *parent):
    QObject(parent)
{}

qint64 Zram::getDiskSize() const
{
    return disksize;
}

qint64 Zram::getData() const
{
    return data;
}

qint64 Zram::getCompressed() const
{
    return compr;
}


void Zram::onZramChanged(qint64 disksize, qint64 data, qint64 compr)
{
    this->disksize = disksize;
    this->data = data;
    this->compr = compr;
    emit zramChanged();
}

}
