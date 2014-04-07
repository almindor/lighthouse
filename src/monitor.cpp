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

#include "monitor.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

namespace Lighthouse {
    const int CPU_FLAGS_ACTIVE = 1;
    const int CPU_FLAGS_INACTIVE = 2;
    const int CPU_PART_COUNT = 10;
    const int CPU_PART_DEF[CPU_PART_COUNT] = {0, 1, 1, 1, 2, 2, 0, 0, 0, 0};

    Monitor::Monitor() : fSettings("Bistrecode", "Lighthouse"), fProcMap()
    {
        fDBus = new QDBusInterface("com.nokia.dsme", "/com/nokia/dsme/request", "com.nokia.dsme.request", QDBusConnection::systemBus(), this);
        fInterval = fSettings.value("proc/interval", 2).toInt();
        fCoverPage = 0;
        fQuit = false;
        fUptime = 0;
        fTicksPerSecond = sysconf(_SC_CLK_TCK);
        fPaused = false;
        start();
    }

    Monitor::~Monitor() {
        delete fDBus;
        fQuit = true;
        wait(2000);
    }

    int Monitor::getInterval() const {
        return fInterval;
    }

    void Monitor::setInterval(int interval) {
        if ( fInterval != interval ) {
            fInterval = interval;
            fSettings.setValue("proc/interval", interval);

            emit intervalChanged(interval);
        }
    }

    void Monitor::setPaused(bool paused) {
        fPaused = paused;
    }

    bool Monitor::getPaused() const {
        return fPaused;
    }    

    void Monitor::setCoverPage(int page) {
        if ( page < 0 ) {
            page = 1;
        } else if ( page > 1 ) {
            page = 0;
        }
        if ( fCoverPage != page ) {
            fCoverPage = page;
            emit coverPageChanged(page);
        }
    }

    int Monitor::getCoverPage() const {
        return fCoverPage;
    }

    QString Monitor::getUptime() const {
        return getUptimeString(fUptime);
    }

    IntList Monitor::getCPUUsage() const {
        return fCPUUsage;
    }

    void Monitor::reboot() {
        fDBus->call("req_reboot");
    }

    void Monitor::shutdown() {
        fDBus->call("req_shutdown");
    }

    void Monitor::run() Q_DECL_OVERRIDE {
        procProcessorCount();
        fCPUActiveTicks.resize(fCPUCount + 1); // room for "total"
        fCPUTotalTicks.resize(fCPUCount + 1); // room for "total"

        fCPUUsage.clear();
        for ( int i = 0; i <= fCPUCount; i++ ) {
            fCPUActiveTicks[i] = 0;
            fCPUTotalTicks[i] = 0;
            fCPUUsage.append(0.0f);
        }

        while (!fQuit) {
            if ( !fPaused ) {
                procUptime();
                procCPUActivity();
                procProcesses();
                procMemory();
            }

            sleep(fInterval);
        }
    }

    void Monitor::procProcessorCount() {
        CPUCountHandler handler(fCPUCount);
        QString path = QStringLiteral("/proc/cpuinfo");
        if ( fProcReader.readProcFile(path, handler, 255, -1) != 0 ) {
            qCritical() << "Error reading processor count\n";
        }
    }

    void Monitor::procCPUActivity() {
        CPUUsageHandler handler(fCPUUsage, fCPUActiveTicks, fCPUTotalTicks);
        QString path = QStringLiteral("/proc/stat");
        if ( fProcReader.readProcFile(path, handler, fCPUCount + 1, -1) == 0 ) {
            emit CPUUsageChanged(&fCPUUsage);
        }
    }

    void Monitor::procMemory() {
        unsigned long free = 0;
        MemoryHandler handler(fTotalMemory, free);

        QString path = QStringLiteral("/proc/meminfo");
        if ( fProcReader.readProcFile(path, handler, 4, -1) == 0 ) {
            emit memoryChanged(fTotalMemory, free);
        }
    }

    void Monitor::procUptime() {
        UptimeHandler handler(fUptime, fUpidle);
        QString path = QStringLiteral("/proc/uptime");
        if ( fProcReader.readProcFile(path, handler, 1, -1) == 0 ) {
            emit uptimeChanged(getUptime());
        } else {
            qCritical() << "Unable to read meminfo\n";
        }
    }

    void Monitor::procProcesses() {
        unsigned long long totalTicks = 0;
        if ( fCPUTotalTicks.size() > 0 ) {
            totalTicks = fCPUTotalTicks[0];
        }

        QStringList procList = fProcReader.getProcList();
        QStringListIterator slIterator(procList);
        while ( slIterator.hasNext() ) {
            int key = slIterator.next().toInt();
            if ( !fProcMap.contains(key) ) {
                fProcMap[key]; // make sure our map has all keys first
            }
        }

        ProcessStatHandler statHandler(fProcMap, totalTicks);
        ProcessStatMHandler statMHandler(fProcMap, fTotalMemory);
        QMapIterator<pid_t, ProcInfo> iterator(fProcMap);
        IntList deletes;

        while ( iterator.hasNext() ) {
            iterator.next();
            pid_t pid = iterator.key();
            QString pathStat("/proc/" + QString::number(pid) + "/stat");
            QString pathStatM("/proc/" + QString::number(pid) + "/statm");
            if ( !QFile::exists(pathStat) ) {
                deletes.append(pid);
            } else {
                if ( fProcReader.readProcFile(pathStat, statHandler, 1, (int)pid) != 0 ) {
                    qCritical() << "Error reading process stat file " << pid << "\n";
                }
                if ( fProcReader.readProcFile(pathStatM, statMHandler, 1, (int)pid) != 0 ) {
                    qCritical() << "Error reading process mstat file " << pid << "\n";
                }
            }
        }

        QListIterator<int> delIterator(deletes);
        while ( delIterator.hasNext() ) {
            fProcMap.remove( delIterator.next() );
        }

        emit processChanged(&fProcMap);
    }

}
