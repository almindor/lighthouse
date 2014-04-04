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

#ifndef PROC_H
#define PROC_H

#include <sys/sysinfo.h>
#include <QThread>
#include <QVector>
#include <QSettings>
#include "types.h"
#include "procreader.h"

namespace Lighthouse {

    class Monitor : public QThread
    {
        public:
            Monitor();
            ~Monitor();
            Q_OBJECT
            IntList getCPUUsage();
            void setInterval(int interval);
            void setPaused(bool paused);
            bool getPaused();
            void setCoverPage(int page);
            int getCoverPage();
            QString getUptime();

            Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
            Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)
            Q_PROPERTY(int coverPage READ getCoverPage WRITE setCoverPage NOTIFY coverPageChanged)
            Q_PROPERTY(QString uptime READ getUptime NOTIFY uptimeChanged)
        private:
            bool fQuit;
            IntList fCPUUsage;
            QLLVector fCPUActiveTicks;
            QLLVector fCPUTotalTicks;
            int fInterval;
            int fCPUCount;
            int fCoverPage;
            qreal fUptime;
            qreal fUpidle;
            long fTicksPerSecond;
            bool fPaused;
            bool fgotBatteryInfo;
            QSettings fSettings;
            ProcMap fProcMap;
            ProcReader fProcReader;
            struct sysinfo fSysInfo;

            void run() Q_DECL_OVERRIDE;
            void procCPUActivity();
            void procMemory();
            void procBattery();
            void procProcesses();
            QString getTimePart(QString key, int value);
            void procUptime();
            void getProcessorCount();
            int getInterval();
        signals:
            void CPUUsageChanged(IntList usage);
            void memoryChanged(int total, int free);
            void intervalChanged(int interval);
            void pausedChanged(bool paused);
            void coverPageChanged(int page);
            void uptimeChanged(QString uptime);
            void processChanged(ProcMap* procMap);
            void batteryHealthChanged(QString heal);
            void batteryTechnoChanged(QString tech);
            void batteryLevelChanged(int level);
            void batteryStatusChanged(QString stat);
    };

}

#endif // PROC_H
