#include "proc.h"
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

    Proc::Proc() : fSettings("Bistrecode", "Lighthouse")
    {
        fInterval = fSettings.value("proc/interval", 2).toInt();
        fCoverPage = 0;
        fQuit = false;
        fTicksPerSecond = sysconf(_SC_CLK_TCK);
        fPaused = false;
        start();
    }

    Proc::~Proc() {
        fQuit = true;
        wait(2000);
    }

    int Proc::getInterval() {
        return fInterval;
    }

    void Proc::setInterval(int interval) {
        if ( fInterval != interval ) {
            fInterval = interval;
            fSettings.setValue("proc/interval", interval);

            emit intervalChanged(interval);
        }
    }

    void Proc::setPaused(bool paused) {
        fPaused = paused;
    }

    bool Proc::getPaused() {
        return fPaused;
    }

    void Proc::setCoverPage(int page) {
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

    int Proc::getCoverPage() {
        return fCoverPage;
    }

    IntList Proc::getCPUUsage() {
        return fCPUUsage;
    }

    void Proc::run() Q_DECL_OVERRIDE {
        fCPUCount = getProcessorCount();
        qDebug() << "CPU count: " << fCPUCount << "\n";
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
                procCPUActivity();
                procMemory();
            }

            sleep(fInterval);
        }
    }

    int Proc::getProcessorCount() {
        CPUCountHandler handler;
        QString path = QStringLiteral("/proc/cpuinfo");
        if ( fProcReader.readProcFile(path, handler, 255) == 0 ) {
            return handler.getCount();
        }

        return 0;
    }

    void Proc::procCPUActivity() {
        CPUUsageHandler handler(fCPUUsage, fCPUActiveTicks, fCPUTotalTicks);
        QString path = QStringLiteral("/proc/stat");
        if ( fProcReader.readProcFile(path, handler, fCPUCount) == 0 ) {
            emit CPUUsageChanged(fCPUUsage);
        }
    }

    void Proc::procMemory() {
        int result = sysinfo(&fSysInfo);

        if ( result != 0 ) {
            qCritical() << "Unable to read sysinfo\n";
            return;
        }

        int total = fSysInfo.totalram * fSysInfo.mem_unit;
        int free = fSysInfo.freeram * fSysInfo.mem_unit;
        emit memoryChanged(total, free);
    }

    void Proc::procUptime() {
        UptimeHandler handler;
        QString path = QStringLiteral("/proc/uptime");
        if ( fProcReader.readProcFile(path, handler, 1) == 0 ) {
            emit uptimeChanged(handler.getUptime(), handler.getUpidle());
        }
    }

}
