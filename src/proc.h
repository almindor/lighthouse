#ifndef PROC_H
#define PROC_H

#include <sys/sysinfo.h>
#include <QThread>
#include <QVariantList>
#include <QVector>

namespace Lighthouse {

    class Proc : public QThread
    {
        public:
            Proc();
            ~Proc();
            Q_OBJECT
            QVariantList getCPUUsage();
            void setInterval(int interval);

            Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
        private:
            bool fQuit;
            QVariantList fCPUUsage;
            QVector<unsigned long long> fCPUActiveTicks;
            QVector<unsigned long long> fCPUTotalTicks;
            int fInterval;
            int fCPUCount;
            struct sysinfo fSysInfo;

            void run() Q_DECL_OVERRIDE;
            void procCPUActivity();
            void procMemory();
            unsigned long long parseCPUParts(QStringList& parts, int flags);
            int getProcessorCount();
            int getInterval();
        signals:
            void CPUUsageChanged(QVariantList usage);
            void memoryChanged(int total, int free);
            void intervalChanged(int interval);
    };

}

#endif // PROC_H
