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
#include <unistd.h>
#include <iostream>
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
        fInterval = fSettings.value("proc/interval", 2).toInt();
        fCoverPage = 0;
        fQuit = false;
        fUptime = 0;
        fTicksPerSecond = sysconf(_SC_CLK_TCK);
        fPaused = false;
        fgotBatteryInfo = false;
        start();
    }

    Monitor::~Monitor() {
        fQuit = true;
        wait(2000);
    }

    int Monitor::getInterval() {
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

    bool Monitor::getPaused() {
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

    int Monitor::getCoverPage() {
        return fCoverPage;
    }

    QString Monitor::getUptime() {
        return getUptimeString(fUptime);
    }

    IntList Monitor::getCPUUsage() {
        return fCPUUsage;
    }

    void Monitor::run() Q_DECL_OVERRIDE {
        getProcessorCount();
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
                procBattery();
            }

            sleep(fInterval);
        }
    }

    void Monitor::getProcessorCount() {
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
            emit CPUUsageChanged(fCPUUsage);
        }
    }

    void Monitor::procMemory() {
        int result = sysinfo(&fSysInfo);

        if ( result != 0 ) {
            qCritical() << "Unable to read sysinfo\n";
            return;
        }

        int total = fSysInfo.totalram * fSysInfo.mem_unit;
        int free = fSysInfo.freeram * fSysInfo.mem_unit;
        emit memoryChanged(total, free);
    }

    void Monitor::procUptime() {
        UptimeHandler handler(fUptime, fUpidle);
        QString path = QStringLiteral("/proc/uptime");
        if ( fProcReader.readProcFile(path, handler, 1, -1) == 0 ) {
            emit uptimeChanged(getUptime());
        } else {
            qCritical() << "Unable to read uptime\n";
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

        ProcessHandler handler(fProcMap, totalTicks);
        QMapIterator<pid_t, ProcInfo> iterator(fProcMap);
        IntList deletes;

        while ( iterator.hasNext() ) {
            iterator.next();
            pid_t pid = iterator.key();
            QString path("/proc/" + QString::number(pid) + "/stat");
            if ( !QFile::exists(path) ) {
                deletes.append(pid);
            } else  if ( fProcReader.readProcFile(path, handler, 1, (int)pid) != 0 ) {
                qCritical() << "Error reading process " << pid << "\n";
            }
        }

        QListIterator<int> delIterator(deletes);
        while ( delIterator.hasNext() ) {
            fProcMap.remove( delIterator.next() );
        }

        emit processChanged(&fProcMap);
    }


    void Monitor::procBattery() {
        if ( !fgotBatteryInfo ){
            QFile f("/sys/class/power_supply/battery/health");
            //QFile f("/home/nemo/power_supply/health");
            if (f.open(QFile::ReadOnly)){
                QByteArray content = f.readAll();
                QString value=QString(content).replace(QString("\n"), QString(""));
                f.close();
                emit batteryHealthChanged(value);
            }

            QFile f1("/sys/class/power_supply/battery/technology");
            //QFile f1("/home/nemo/power_supply/technology");
            if (f1.open(QFile::ReadOnly)){
                QByteArray content = f1.readAll();
                QString value=QString(content).replace(QString("\n"), QString(""));
                f1.close();
                emit batteryTechnoChanged(value);
            }
            fgotBatteryInfo=true;
        }
        QFile f2("/sys/class/power_supply/battery/capacity");
        //QFile f2("/home/nemo/power_supply/capacity");
        if (f2.open(QFile::ReadOnly)){
            QByteArray content = f2.readAll();
            int v = QString(content).toInt();
            f2.close();
            emit batteryLevelChanged(v);
        }
        QFile f3("/sys/class/power_supply/battery/status");
        //QFile f3("/home/nemo/power_supply/status");
        if (f3.open(QFile::ReadOnly)){
            QByteArray content = f3.readAll();//.replace(QString("\n"), QString(""));
            QString value=QString(content).replace(QString("\n"), QString(""));
            f3.close();
            emit batteryStatusChanged(value);
        }
    }

}
