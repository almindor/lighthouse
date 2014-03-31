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

    class ProcessHandler: public LineHandler
    {
    public:
        ProcessHandler(ProcMap& procMap, unsigned long long totalTicks);
        int onLine(QString& line, int i);
    private:
        unsigned long long fTotalTicks;
        ProcMap& fProcMap;
    };

}

#endif // LINEHANDLER_H
