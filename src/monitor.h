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

#include <QThread>
#include <QVector>
#include <QSettings>
#include <QtDBus/QDBusInterface>
#include "types.h"
#include "procreader.h"

namespace Lighthouse {

    class Monitor : public QThread
    {
        public:
            Monitor();
            ~Monitor();
            Q_OBJECT
            IntList getCPUUsage() const;
            void setInterval(int interval);
            void setPaused(bool paused);
            bool getPaused() const;
            void setCoverPage(int page);
            int getCoverPage() const;
            const QString& getCoverLabel() const;
            QString getUptime() const;
            const QString& getCoverImageLeft() const;
            const QString& getCoverImageRight() const;
            Q_INVOKABLE void reboot();
            Q_INVOKABLE void shutdown();
            Q_INVOKABLE void setProcessDetails(bool active);
            Q_INVOKABLE void setApplicationActive(bool active);

            Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
            Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged)
            Q_PROPERTY(int coverPage READ getCoverPage WRITE setCoverPage NOTIFY coverPageChanged)
            Q_PROPERTY(QString coverLabel READ getCoverLabel NOTIFY coverPageChanged)
            Q_PROPERTY(QString coverImageLeft READ getCoverImageLeft NOTIFY coverPageChanged)
            Q_PROPERTY(QString coverImageRight READ getCoverImageRight NOTIFY coverPageChanged)
            Q_PROPERTY(QString uptime READ getUptime NOTIFY uptimeChanged)
        private:
            bool fQuit;
            IntList fCPUUsage;
            QLLVector fCPUActiveTicks;
            QLLVector fCPUTotalTicks;
            int fInterval;
            int fCPUCount;
            int fCoverPage;
            unsigned long fTotalMemory;
            qreal fUptime;
            qreal fUpidle;
            long fTicksPerSecond;
            bool fPaused;
            bool fGotBatteryInfo;
            QSettings fSettings;
            ProcMap fProcMap;
            ProcReader fProcReader;
            QDBusInterface* fDBus;
            AppNameMap fAppNameMap;
            bool fApplicationActive;
            bool fProcessDetails;

            QString getAppName(const QString& fileName) const;
            void fillApplicationMap();
            void run() Q_DECL_OVERRIDE;
            void procCPUActivity();
            void procMemory();
            void procBattery();
            void procProcessCount();
            void procProcesses();
            void procUptime();
            void procTemperature();
            void procProcessorCount();
            void fillProcMap(ProcMap& procMap, IntList* deletes);
            int getInterval() const;
        public slots:
            void updateApplicationMap(const QString& path);
        signals:
            void CPUUsageChanged(const IntList& usage);
            void memoryChanged(unsigned long total, unsigned long free);
            void intervalChanged(int interval);
            void pausedChanged(bool paused);
            void coverPageChanged(int page);
            void uptimeChanged(QString uptime);
            void processCountChanged(int count);
            void processChanged(const ProcMap* procMap, const PIDList& adds, const PIDList& deletes);
            void batteryHealthChanged(QString heal);
            void batteryTechnologyChanged(QString tech);
            void batteryLevelChanged(int level);
            void temperatureChanged(int degrees);
            void batteryStatusChanged(QString stat);
    };

}

#endif // PROC_H
