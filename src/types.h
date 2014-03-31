#ifndef TYPES_H
#define TYPES_H

#include <QObject>
#include <QString>
#include <QMap>

namespace Lighthouse {

    typedef QList<int> IntList;
    typedef QVector<unsigned long long> QLLVector;

    class ProcInfo
    {
        /*Q_OBJECT
        Q_PROPERTY(qreal CPUUsage READ getCPUUsage)
        Q_PROPERTY(pid_t PID READ getPID)
        Q_PROPERTY(QString name READ getName)*/
    public:
        ProcInfo();
        void update(QString& stat, unsigned long long totalTicks);
        QString getName() const;
        pid_t getPID() const;
        qreal getCPUUsage() const;
        QString toString() const;
    private:
        QString fName;
        pid_t fPID;
        qreal fCPUUsage;
        QString fState;
        unsigned long fSysTime;
        unsigned long fUserTime;
        unsigned long fTotalTicks;
    };

    typedef QMap<pid_t, ProcInfo> ProcMap;
    typedef QList<ProcInfo> ProcList;

    QString getUptimeString(qreal uptime);

    struct CPUComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const
        {
            return a.getCPUUsage() > b.getCPUUsage();
        }
    };

    struct MemoryComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const
        {
            return a.getCPUUsage() > b.getCPUUsage();
        }
    };

    struct NameComparer
    {
        bool operator()(const ProcInfo & a, const ProcInfo & b) const
        {
            return a.getName() < b.getName();
        }
    };

}

#endif // TYPES_H
