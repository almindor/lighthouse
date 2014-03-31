#include "types.h"
#include <QStringList>
#include <QTextStream>
#include <QRegularExpression>
#include <math.h>

namespace Lighthouse {

    ProcInfo::ProcInfo() : fName() {
        fPID = 0;
        fCPUUsage = 0;
        fState = 'Q';
        fUserTime = 0;
        fSysTime = 0;
        fTotalTicks = 0;
    }

    void ProcInfo::update(QString& stat, unsigned long long totalTicks) {
        unsigned long oldCPUTime = fUserTime + fSysTime;
        int tmp;
        QTextStream(&stat) >> fPID >> fName >> fState >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> fUserTime >> fSysTime;

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

    qreal ProcInfo::getCPUUsage() const {
        return fCPUUsage;
    }

    QString ProcInfo::toString() const {
        const QRegularExpression regexp("[()]+");
        QString name(fName);
        QString result(name.replace(regexp, "") + " [" + fState + "] CPU: " + QString::number(fCPUUsage) + "\n");
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


}
