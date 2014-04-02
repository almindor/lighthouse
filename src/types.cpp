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

#include "types.h"
#include <QStringList>
#include <QTextStream>
#include <QRegularExpression>
#include <math.h>

namespace Lighthouse {

    bool CPUComparer::operator()(const ProcInfo & a, const ProcInfo & b) const {
        if ( a.getCPUUsage() == b.getCPUUsage() ) {
            return a.getPID() > b.getPID();
        }

        return a.getCPUUsage() > b.getCPUUsage();
    }

    bool MemoryComparer::operator()(const ProcInfo & a, const ProcInfo & b) const {
        if ( a.getCPUUsage() == b.getCPUUsage() ) {
            return a.getPID() > b.getPID();
        }

        return a.getCPUUsage() > b.getCPUUsage();
    }

    bool NameComparer::operator()(const ProcInfo & a, const ProcInfo & b) const {
        return (QString::compare(a.getName(), b.getName(), Qt::CaseInsensitive) < 0);
    }

    // ProcInfo

    ProcInfo::ProcInfo() : fName() {
        fPID = 0;
        fCPUUsage = 0;
        fState = 'Q';
        fUserTime = 0;
        fSysTime = 0;
        fTotalTicks = 0;
    }

    void ProcInfo::update(QString& stat, unsigned long long totalTicks) {
        const QRegularExpression regexp("[()]+");
        unsigned long oldCPUTime = fUserTime + fSysTime;
        int tmp;
        QTextStream(&stat) >> fPID >> fName >> fState >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> fUserTime >> fSysTime;
        fName = fName.replace(regexp, "");

        if ( fTotalTicks > 0 ) {
            unsigned long newCPUTime = fUserTime + fSysTime;
            unsigned long diffCPUTime = newCPUTime - oldCPUTime;
            unsigned long diffTotalTicks = totalTicks - fTotalTicks;
            fCPUUsage = round((qreal)diffCPUTime / (qreal)diffTotalTicks * 100.0f);
        }

        fTotalTicks = totalTicks;
    }

    QString ProcInfo::getName() const {
        return fName;
    }

    pid_t ProcInfo::getPID() const {
        return fPID;
    }

    int ProcInfo::getCPUUsage() const {
        return fCPUUsage;
    }

    int ProcInfo::getMemoryUsage() const {
        return 0; // TODO
    }

    QString ProcInfo::toString() const {
        QString result(fName + "\tCPU: " + QString::number(fCPUUsage) + "%\n");
        return result;
    }

    QString getTimePart(QString key, int value) {
        if ( value == 0 ) {
            return "";
        }

        return QString::number(value) + key;// + (value > 1 ? "s" : "");
    }

    QString getUptimeString(qreal uptime) {
        qint64 seconds = qRound64(uptime);
        int years = 0;
        int months = 0;
        int days = 0;
        int hours = 0;
        int minutes = 0;

        if ( seconds > 60 * 60 * 24 * 30 * 12 ) {
            years = seconds / (60 * 60 * 24 * 30 * 12);
            seconds %= (60 * 60 * 24 * 30 * 12);
        }

        if ( seconds > 60 * 60 * 24 * 30 ) {
            months = seconds / (60 * 60 * 24 * 30);
            seconds %= (60 * 60 * 24 * 30);
        }

        if ( seconds > 60 * 60 * 24 ) {
            days = seconds / (60 * 60 * 24);
            seconds %= (60 * 60 * 24);
        }

        if ( seconds > 60 * 60 ) {
            hours = seconds / (60 * 60);
            seconds %= (60 * 60);
        }

        if ( seconds > 60 ) {
            minutes = seconds / 60;
            seconds %= 60;
        }

        QStringList parts;
        parts.append(getTimePart("y", years));
        parts.append(getTimePart("mo", months));
        parts.append(getTimePart("d", days));
        parts.append(getTimePart("h", hours));
        parts.append(getTimePart("m", minutes));
        parts.append(getTimePart("s", seconds));

        parts.removeDuplicates();

        return parts.join(" ");
    }

    bool ProcInfo::operator ==(const ProcInfo& other) const {
        return ( fPID == other.getPID() && fCPUUsage == other.getCPUUsage() );
    }

}
