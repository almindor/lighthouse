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
#include <QDebug>

#include <sys/types.h>
#include <unistd.h>

#define PAGE_SIZE 4096 // TODO: find out dynamicallys

namespace Lighthouse {

    void BaseComparer::setProcMap(const ProcMap* procMap) {
        fProcMap = procMap;
    }

    bool CPUComparer::operator()(const pid_t a, const pid_t b) const {
        int valA = fProcMap->value(a).getCPUUsage();
        int valB = fProcMap->value(b).getCPUUsage();
        if ( valA == valB ) {
            return fProcMap->value(a).getPID() > fProcMap->value(b).getPID();
        }

        return valA > valB;
    }

    bool MemoryComparer::operator()(const pid_t a, const pid_t b) const {
        int valA = fProcMap->value(a).getMemoryUsage();
        int valB = fProcMap->value(b).getMemoryUsage();
        if ( valA == valB ) {
            return fProcMap->value(a).getPID() > fProcMap->value(b).getPID();
        }

        return valA > valB;
    }

    bool NameComparer::operator()(const pid_t a, const pid_t b) const {
        return (QString::compare(fProcMap->value(a).getName(), fProcMap->value(b).getName(), Qt::CaseInsensitive) < 0);
    }

    // ProcInfo

    ProcInfo::ProcInfo() : fApplicationName(), fStatName(), fShowName() {
        fPID = 0;
        fCPUUsage = 0;
        fMemoryUsage = 0;
        fState = 'Q';
        fUserTime = 0;
        fSysTime = 0;
        fTotalTicks = 0;
        fVmSize = 0;
        fVmRSS = 0;
        fSharedMem = 0;
        fNameState = 0;
    }

    void ProcInfo::updateStat(QString& stat, unsigned long long totalTicks) {
        unsigned long oldCPUTime = fUserTime + fSysTime;
        int tmp;

        QTextStream(&stat) >> fPID >> fStatName >> fState >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> fUserTime >> fSysTime;
        setShowName(fStatName, 0);

        if ( fTotalTicks > 0 ) {
            unsigned long newCPUTime = fUserTime + fSysTime;
            unsigned long diffCPUTime = newCPUTime - oldCPUTime;
            unsigned long diffTotalTicks = totalTicks - fTotalTicks;
            fCPUUsage = round((qreal)diffCPUTime / (qreal)diffTotalTicks * 100.0f);
        }

        fTotalTicks = totalTicks;
    }

    void ProcInfo::updateMemory(QString& mem, unsigned long totalMemory) {
        QTextStream(&mem) >> fVmSize >> fVmRSS >> fSharedMem;
        fVmSize *= PAGE_SIZE;
        fVmRSS *= PAGE_SIZE;
        fSharedMem *= PAGE_SIZE;

        fMemoryUsage = round((qreal)fVmRSS / ((qreal)totalMemory * 1000.0f) * 100.0f);
    }

    void ProcInfo::updateApplicationName(QString& appName) {
        fApplicationName = appName;

        setShowName(appName, 2);
    }

    void ProcInfo::updateName(QString& appName) {
        setShowName(appName, 1);
    }

    static const QRegExp BRACES_REGEXP("[()]*");
    static const int MAX_SHOW_NAME_SIZE = 13;

    void ProcInfo::setShowName(QString& source, int nameState) {
        if ( fNameState > nameState ) {
            return; // we have a name now
        }

        fNameState = nameState;
        fShowName = source;
        if ( fShowName[0] == '(' ) {
            fShowName.replace(BRACES_REGEXP, "");
        }

        if ( fShowName.size() > MAX_SHOW_NAME_SIZE ) {
            fShowName = fShowName.left(MAX_SHOW_NAME_SIZE);
        }
    }

    const QString& ProcInfo::getName() const {
        return fShowName;
    }

    pid_t ProcInfo::getPID() const {
        return fPID;
    }

    int ProcInfo::getCPUUsage() const {
        return fCPUUsage;
    }

    int ProcInfo::getMemoryUsage() const {
        return fMemoryUsage;
    }

    int ProcInfo::getNameState() const {
        return fNameState;
    }

    bool ProcInfo::isApplication() const {
        return !fApplicationName.isEmpty();
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
        return ( fPID == other.getPID() && fCPUUsage == other.getCPUUsage() && fMemoryUsage == other.getMemoryUsage() );
    }

}
