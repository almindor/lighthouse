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
        virtual int onLine(QString& line, int i);
        qreal getUptime();
        qreal getUpidle();
    private:
        qreal fUptime;
        qreal fUpidle;
    };

    class CPUCountHandler: public LineHandler
    {
    public:
        CPUCountHandler();
        virtual int onLine(QString& line, int i);
        int getCount();
    private:
        int fCount;
    };

    class CPUUsageHandler: public LineHandler
    {
    public:
        CPUUsageHandler(IntList& usage, QLLVector& activeTicks, QLLVector& totalTicks);
        virtual int onLine(QString& line, int i);
    private:
        IntList& fCPUUsage;
        QLLVector& fCPUActiveTicks;
        QLLVector& fCPUTotalTicks;

        unsigned long long parseCPUParts(QStringList &parts, int flags);
    };

}

#endif // LINEHANDLER_H
