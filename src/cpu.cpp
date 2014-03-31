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

#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    CPU::CPU(QObject *parent) :
        QObject(parent)
    {

    }

    IntList CPU::getUsage() const {
        return fUsage;
    }

    int CPU::getSummaryValue() {
        if ( fUsage.size() < 1 ) {
            return 0;
        }

        return fUsage[0];
    }

    void CPU::setUsage(IntList usage) {
        if ( usage.size() != fUsage.size() || fUsage != usage ) {
            fUsage = usage;
            emit usageChanged();
            emit summaryValueChanged();
        }
    }

}
