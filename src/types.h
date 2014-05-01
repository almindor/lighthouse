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

#ifndef TYPES_H
#define TYPES_H

#include <QObject>
#include <QString>
#include <QMap>

namespace Lighthouse {

    typedef QList<int> IntList;
    typedef QVector<unsigned long long> QLLVector;
    typedef QMap<QString, QString> AppNameMap;

    class ProcInfo
    {
    public:
        ProcInfo();
        void updateBegin();
        void updateStat(QString& stat, unsigned long long totalTicks);
        void updateMemory(QString& mem, unsigned long totalMemory);
        void updateApplicationName(QString& appName);
        void updateName(QString& appName);

        const QString& getName() const;
        pid_t getPID() const;
        int getCPUUsage() const;
        int getMemoryUsage() const;
        int getNameState() const;
        bool getChanged() const;
        bool isApplication() const;

        bool operator ==(const ProcInfo& other) const;
    private:
        QString fApplicationName;
        QString fStatName;
        QString fShowName;
        pid_t fPID;
        int fCPUUsage;
        int fMemoryUsage;
        QString fState;
        int fNameState;
        bool fChanged;
        unsigned long fVmSize;
        unsigned long fVmRSS;
        unsigned long fSharedMem;
        unsigned long fSysTime;
        unsigned long fUserTime;
        unsigned long fTotalTicks;

        void setShowName(QString& source, int nameState);
    };

    typedef QMap<pid_t, ProcInfo> ProcMap;
    typedef QList<ProcInfo> ProcList;

    QString getUptimeString(qreal uptime);

    struct CPUComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const;
    };

    struct MemoryComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const;
    };

    struct NameComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const;
    };

}

#endif // TYPES_H
