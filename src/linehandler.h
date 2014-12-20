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

#ifndef LINEHANDLER_H
#define LINEHANDLER_H

#include <QString>
#include "types.h"

namespace Lighthouse {

    class LineHandler
    {
    public:
        virtual int onLine(QString& line, int i) = 0;
    };

    class UptimeHandler: public LineHandler
    {
    public:
        UptimeHandler(qreal& uptime, qreal& upidle);
        int onLine(QString& line, int i);
    private:
        qreal& fUptime;
        qreal& fUpidle;
    };

    class MemoryHandler: public LineHandler
    {
    public:
        MemoryHandler(unsigned long& total, unsigned long& free);
        int onLine(QString& line, int i);
    private:
        unsigned long& fTotal;
        unsigned long& fFree;
    };

    class CPUCountHandler: public LineHandler
    {
    public:
        CPUCountHandler(int& count);
        int onLine(QString& line, int i);
    private:
        int& fCount;
    };

    class CPUUsageHandler: public LineHandler
    {
    public:
        CPUUsageHandler(IntList& usage, QLLVector& activeTicks, QLLVector& totalTicks);
        int onLine(QString& line, int i);
    private:
        IntList& fCPUUsage;
        QLLVector& fCPUActiveTicks;
        QLLVector& fCPUTotalTicks;

        unsigned long long parseCPUParts(QStringList &parts, int flags);
    };

    class ProcessStatHandler: public LineHandler
    {
    public:
        ProcessStatHandler(ProcMap& procMap, unsigned long long totalTicks);
        int onLine(QString& line, int i);
    private:
        unsigned long long fTotalTicks;
        ProcMap& fProcMap;
    };

    class ProcessStatMHandler: public LineHandler
    {
    public:
        ProcessStatMHandler(ProcMap& procMap, unsigned long totalMemory);
        int onLine(QString& line, int i);
    private:
        unsigned long fTotalMemory;
        ProcMap& fProcMap;
    };

    class ProcessCmdLineHandler: public LineHandler
    {
    public:
        ProcessCmdLineHandler(ProcMap& procMap, AppNameMap& appNameMap);
        int onLine(QString& line, int i);
    private:
        ProcMap& fProcMap;
        AppNameMap& fAppNameMap;
    };


}

#endif // LINEHANDLER_H
