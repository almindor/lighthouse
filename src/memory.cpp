/*
    Copyright (C) 2014 Ales Katona.

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

#include "memory.h"
#include <QDebug>

namespace Lighthouse {

    Memory::Memory(QObject *parent) :
        QObject(parent)
    {
        fTotal = 0;
        fFree = 0;
    }

    int Memory::getFree() const {
        return fFree;
    }

    int Memory::getTotal() const {
        return fTotal;
    }

    int Memory::getSummaryValue() const {
        return 100 - qRound((qreal)fFree / (qreal)fTotal * 100.0f);
    }

    void Memory::setMemory(int total, int free) {
        int changed = 0;
        if ( fTotal != total ) {
            fTotal = total;
            emit totalChanged(total);
            changed++;
        }

        if ( fFree != free ) {
            fFree = free;
            emit freeChanged(free);
            changed++;
        }

        if ( changed > 0 ) {
            emit summaryValueChanged(getSummaryValue());
        }
    }

}
