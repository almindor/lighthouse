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

#include "linehandler.h"
#include <QStringList>
#include <QDebug>

namespace Lighthouse {

    const int CPU_FLAGS_ACTIVE = 1;
    const int CPU_FLAGS_INACTIVE = 2;
    const int CPU_PART_COUNT = 10;
    const int CPU_PART_DEF[CPU_PART_COUNT] = {0, 1, 1, 1, 2, 2, 0, 0, 0, 0};

    UptimeHandler::UptimeHandler(qreal &uptime, qreal &upidle) : fUptime(uptime), fUpidle(upidle) {

    }

    int UptimeHandler::onLine(QString &line, int i) {
        QStringList parts = line.split(" ", QString::SkipEmptyParts);
        if ( parts.size() < 2 ) {
            qCritical() << "Contents of /proc/uptime invalid on line " << i << "\n";
            return -1;
        }

        fUptime = parts[0].toFloat();
        fUpidle = parts[1].toFloat();
        return 0;
    }

    CPUCountHandler::CPUCountHandler(int& count) : fCount(count) {
        fCount = 0;
    }

    int CPUCountHandler::onLine(QString &line, int i) {
        if ( i > 255 ) {
            return -1;
        }
        if ( line.contains("processor") ) {
            fCount++;
        }

        return 0;
    }

    CPUUsageHandler::CPUUsageHandler(IntList& usage, QLLVector& activeTicks, QLLVector& totalTicks)
                                    : fCPUUsage(usage), fCPUActiveTicks(activeTicks), fCPUTotalTicks(totalTicks) {

    }

    int CPUUsageHandler::onLine(QString &line, int i) {
        unsigned long long diffActiveTicks;
        unsigned long long diffTotalTicks;
        unsigned long long oldActiveTicks;
        unsigned long long oldTotalTicks;
        qreal usage;

        QStringList parts = line.split(" ", QString::SkipEmptyParts);
        if ( parts.size() < 8 ) {
            qCritical() << "Not enough parts in CPU line: " << line << "\n";
            return -1;
        }

        QString type = parts.at(0);
        if ( !type.contains("cpu") ) {
            qWarning() << "Invalid CPU type indicator encountered: " << type;
            return -1;
        }

        oldActiveTicks = fCPUActiveTicks[i];
        fCPUActiveTicks[i] = parseCPUParts(parts, CPU_FLAGS_ACTIVE);
        diffActiveTicks = fCPUActiveTicks[i] - oldActiveTicks;

        oldTotalTicks = fCPUTotalTicks[i];
        fCPUTotalTicks[i] = parseCPUParts(parts, -1);
        diffTotalTicks = fCPUTotalTicks[i] - oldTotalTicks;

        usage = (qreal)diffActiveTicks / (qreal)diffTotalTicks * 100.0f;
        int iUsage = qRound(usage);

        if ( iUsage != fCPUUsage[i] ) {
            fCPUUsage[i] = iUsage;
        }

        return 0;
    }

    unsigned long long CPUUsageHandler::parseCPUParts(QStringList &parts, int flags) {
        bool converted = true;
        unsigned long long result = 0;

        for ( int i = 1; i < CPU_PART_COUNT; i++ ) {
            int flag = CPU_PART_DEF[i];
            QString value = parts.at(i);
            if ( flags < 0 || flag == flags ) {
                result += value.toULongLong(&converted);
            }
            if ( !converted ) {
                qCritical() << "Unable to convert " << value << " to unsigned long long\n";
                return 0;
            }
        }

        return result;
    }

    ProcessStatHandler::ProcessStatHandler(ProcMap& procMap, unsigned long long totalTicks) : fProcMap(procMap) {
        fTotalTicks = totalTicks;
    }

    int ProcessStatHandler::onLine(QString &line, int i) {
        pid_t index = i;
        fProcMap[index].updateStat(line, fTotalTicks);
        return 0;
    }

    ProcessStatMHandler::ProcessStatMHandler(ProcMap& procMap, unsigned long long totalMemory) : fProcMap(procMap) {
        fTotalMemory = totalMemory;
    }

    int ProcessStatMHandler::onLine(QString &line, int i) {
        pid_t index = i;
        fProcMap[index].updateMemory(line, fTotalMemory);
        return 0;
    }

}
