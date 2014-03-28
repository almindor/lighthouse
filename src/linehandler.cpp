#include "linehandler.h"
#include <QStringList>
#include <QDebug>

namespace Lighthouse {

    const int CPU_FLAGS_ACTIVE = 1;
    const int CPU_FLAGS_INACTIVE = 2;
    const int CPU_PART_COUNT = 10;
    const int CPU_PART_DEF[CPU_PART_COUNT] = {0, 1, 1, 1, 2, 2, 0, 0, 0, 0};

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

    qreal UptimeHandler::getUptime() {
        return fUptime;
    }

    qreal UptimeHandler::getUpidle() {
        return fUpidle;
    }

    CPUCountHandler::CPUCountHandler() {
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

    int CPUCountHandler::getCount() {
        return fCount;
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


}
