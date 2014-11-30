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
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDir>

namespace Lighthouse {
    const int CPU_FLAGS_ACTIVE = 1;
    const int CPU_FLAGS_INACTIVE = 2;
    const int CPU_PART_COUNT = 10;
    const int CPU_PART_DEF[CPU_PART_COUNT] = {0, 1, 1, 1, 2, 2, 0, 0, 0, 0};

    Monitor::Monitor() : fSettings(), fProcMap(), fAppNameMap()
    {
        fDBus = new QDBusInterface("com.nokia.dsme", "/com/nokia/dsme/request", "com.nokia.dsme.request", QDBusConnection::systemBus(), this);
        fInterval = fSettings.value("proc/interval", 2).toInt();
        fCoverPage = 0;
        fQuit = false;
        fUptime = 0;
        fTicksPerSecond = sysconf(_SC_CLK_TCK);
        fPaused = false;
        fGotBatteryInfo = false;
        fApplicationActive = false;
        fProcessDetails = false;
        fBadTicks = false; // work around for bug introduced in Jolla/SailfishOS 1.0.7.16
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
            page = 2;
        } else if ( page > 2 ) {
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

    static const QString COVER_IMAGE_CPU = QStringLiteral("cpu");
    static const QString COVER_IMAGE_MEMORY = QStringLiteral("memory");
    static const QString COVER_IMAGE_BATTERY = QStringLiteral("battery");
    static const QString COVER_IMAGE_UNKNOWN = QStringLiteral("unknown");

    const QString& Monitor::getCoverImageLeft() const {
        switch ( fCoverPage ) {
            case 0: return COVER_IMAGE_BATTERY;
            case 1: return COVER_IMAGE_CPU;
            case 2: return COVER_IMAGE_MEMORY;
        }

        return COVER_IMAGE_UNKNOWN;
    }

    const QString& Monitor::getCoverImageRight() const {
        switch ( fCoverPage ) {
            case 0: return COVER_IMAGE_MEMORY;
            case 1: return COVER_IMAGE_BATTERY;
            case 2: return COVER_IMAGE_CPU;
        }

        return COVER_IMAGE_UNKNOWN;
    }

    static const QString COVER_LABEL_CPU = QStringLiteral("CPU");
    static const QString COVER_LABEL_MEMORY = QStringLiteral("Memory");
    static const QString COVER_LABEL_BATTERY = QStringLiteral("Battery");
    static const QString COVER_LABEL_UNKNOWN = QStringLiteral("Unknown");

    const QString& Monitor::getCoverLabel() const {
        switch ( fCoverPage ) {
            case 0: return COVER_LABEL_CPU;
            case 1: return COVER_LABEL_MEMORY;
            case 2: return COVER_LABEL_BATTERY;
        }

        return COVER_LABEL_UNKNOWN;
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

    void Monitor::setProcessDetails(bool active) {
        fProcessDetails = active;
    }

    void Monitor::setApplicationActive(bool active) {
        fApplicationActive = active;
    }

    void Monitor::run() Q_DECL_OVERRIDE {
        updateApplicationMap("/usr/share/applications");
        procProcessorCount();
        fCPUActiveTicks.resize(fCPUCount + 1); // room for "total"
        fCPUTotalTicks.resize(fCPUCount + 1); // room for "total"

        fCPUUsage.clear();
        for ( int i = 0; i <= fCPUCount; i++ ) {
            fCPUActiveTicks[i] = 0;
            fCPUTotalTicks[i] = 0;
            fCPUUsage.append(0.0f);
        }

        unsigned long iteration = 0;
        while (!fQuit) {
            if ( !fPaused ) {
                procCPUActivity();
                procMemory();
                procBattery();

                if ( fApplicationActive || iteration == 0 ) {
                    procUptime();
                    if ( fProcessDetails || iteration == 0 ) {
                        procProcesses();
                    } else {
                        procProcessCount();
                    }
                    procTemperature();
                }
            }

            sleep(fInterval);
            iteration++;
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
        CPUUsageHandler handler(fCPUUsage, fCPUActiveTicks, fCPUTotalTicks, fBadTicks);
        QString path = QStringLiteral("/proc/stat");
        if ( fProcReader.readProcFile(path, handler, fCPUCount + 1, -1) == 0 ) {
            emit CPUUsageChanged(fCPUUsage);
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

    void Monitor::procProcessCount() {
        const int count = fProcReader.getProcList().size();
        emit processCountChanged(count);
    }

    void Monitor::procProcesses() {       
        PIDList deletes;
        PIDList adds;
        unsigned long long totalTicks = 0;

        if ( fBadTicks ) {
            qWarning() << "Skipping processess due to bad ticks from /proc/stat\n";
            return;
        }

        if ( fCPUTotalTicks.size() > 0 ) {
            totalTicks = fCPUTotalTicks[0];
        }

        ProcessStatHandler statHandler(fProcMap, totalTicks);
        ProcessStatMHandler statMHandler(fProcMap, fTotalMemory);
        ProcessCmdLineHandler cmdLineHandler(fProcMap, fAppNameMap);

        QStringList procList = fProcReader.getProcList();
        QStringListIterator slIterator(procList);
        while ( slIterator.hasNext() ) {
            const pid_t pid = slIterator.next().toInt();
            if ( !fProcMap.contains(pid) ) {
                fProcMap[pid];
                adds.append(pid);
            }
        }

        QMapIterator<pid_t, ProcInfo> iterator(fProcMap);
        while ( iterator.hasNext() ) {
            iterator.next();
            const pid_t pid = iterator.key();
            const QString pidStr = QString::number(pid);
            QString pathStat("/proc/" + pidStr + "/stat");
            QString pathStatM("/proc/" + pidStr + "/statm");
            QString pathCmdLine("/proc/" + pidStr + "/cmdline");

            if ( !QFile::exists(pathStat) ) {
                deletes.append(pid);
                fProcMap.remove(pid);
            } else {
                if ( fProcReader.readProcFile(pathStat, statHandler, 1, pid) != 0 ) {
                    qCritical() << "Error reading process stat file " << pid << "\n";
                }
                if ( fProcReader.readProcFile(pathStatM, statMHandler, 1, pid) != 0 ) {
                    qCritical() << "Error reading process mstat file " << pid << "\n";
                }

                if ( iterator.value().getNameState() == 0 ) {
                    if ( fProcReader.readProcFile(pathCmdLine, cmdLineHandler, 1, pid) != 0 ) {
                        qCritical() << "Error reading process cmdline file " << pid << "\n";
                    }
                }
            }
        }

        emit processChanged(&fProcMap, adds, deletes);
    }

    void Monitor::procBattery() {
        QString value;
        if ( !fGotBatteryInfo ){
            QFile f("/sys/class/power_supply/battery/health");
            if (f.open(QFile::ReadOnly)){
                QByteArray content = f.readAll();
                value = QString(content).replace(QString("\n"), QString(""));
                f.close();
                emit batteryHealthChanged(value);
            }

            QFile f1("/sys/class/power_supply/battery/technology");
            if (f1.open(QFile::ReadOnly)){
                QByteArray content = f1.readAll();
                value = QString(content).replace(QString("\n"), QString(""));
                f1.close();
                emit batteryTechnologyChanged(value);
            }
            fGotBatteryInfo = true;
        }

        QFile f2("/sys/class/power_supply/battery/capacity");
        if (f2.open(QFile::ReadOnly)){
            QByteArray content = f2.readAll();
            int v = QString(content).toInt();
            f2.close();
            emit batteryLevelChanged(v);
        }
        QFile f3("/sys/class/power_supply/battery/status");
        if (f3.open(QFile::ReadOnly)){
            QByteArray content = f3.readAll();
            value = QString(content).replace(QString("\n"), QString(""));
            f3.close();
            emit batteryStatusChanged(value);
        }
    }

    void Monitor::procTemperature() {
        QFile f("/sys/class/thermal/thermal_zone0/temp");
        if ( f.open(QFile::ReadOnly) ) {
            QByteArray content = f.readAll();
            QString value = QString(content).replace("\n", "");
            emit temperatureChanged(value.toInt());
        }
    }

    QString Monitor::getAppName(const QString& fileName) const {
        const QString fullName = "/usr/share/applications/" + fileName;
        if ( !QFile::exists(fullName) ) {
            qWarning() << "File not found: " << fullName << "\n";
            return QString();
        }

        QSettings desktop(fullName, QSettings::IniFormat);
        desktop.setIniCodec("UTF-8");
        const QString name = desktop.value("Desktop Entry/Name", "Unknown").toString();
        if ( name == "Unknown" ) {
            //qWarning() << "App name not found: " << fullName << "\n";
            return QString();
        }
        return name;
    }

    void Monitor::updateApplicationMap(const QString& path) {
        QDir apps(path); // "/usr/share/applications"
        QStringList filters;
        filters << "*.desktop";
        apps.setNameFilters(filters);

        const QStringList files = apps.entryList();
        QStringListIterator iterator(files);
        fAppNameMap.clear();
        while ( iterator.hasNext() ) {
            const QString& fileName = iterator.next();
            QString baseName(fileName);
            baseName.replace(".desktop", "");
            const QString appName = getAppName(fileName);
            if ( !appName.isEmpty() ) {
                fAppNameMap[baseName] = appName;
            }
        }
    }

}
