#ifndef PROC_H
#define PROC_H

#include <sys/sysinfo.h>
#include <QThread>
#include <QVector>
#include <QSettings>
#include "types.h"
#include "procreader.h"

namespace Lighthouse {

    class Proc : public QThread
    {
        public:
            Proc();
            ~Proc();
            Q_OBJECT
            IntList getCPUUsage();
            void setInterval(int interval);
            void setPaused(bool paused);
            bool getPaused();
            void setCoverPage(int page);
            int getCoverPage();

            Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
            Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)
            Q_PROPERTY(int coverPage READ getCoverPage WRITE setCoverPage NOTIFY coverPageChanged)
        private:
            bool fQuit;
            IntList fCPUUsage;
            QLLVector fCPUActiveTicks;
            QLLVector fCPUTotalTicks;
            int fInterval;
            int fCPUCount;
            int fCoverPage;
            long fTicksPerSecond;
            bool fPaused;
            QSettings fSettings;
            ProcReader fProcReader;
            struct sysinfo fSysInfo;

            void run() Q_DECL_OVERRIDE;
            void procCPUActivity();
            void procMemory();
            void procUptime();
            int getProcessorCount();
            int getInterval();
        signals:
            void CPUUsageChanged(IntList usage);
            void memoryChanged(int total, int free);
            void intervalChanged(int interval);
            void pausedChanged(bool paused);
            void coverPageChanged(int page);
            void uptimeChanged(qreal uptime, qreal upidle);
    };

}

#endif // PROC_H
