#include "proc.h"
#include <iostream>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

namespace Lighthouse {

    const int CPU_FLAGS_ACTIVE = 1;
    const int CPU_FLAGS_INACTIVE = 2;
    const int CPU_PART_COUNT = 10;
    const int CPU_PART_DEF[CPU_PART_COUNT] = {0, 1, 1, 1, 2, 2, 1, 1, 0, 0};

    Proc::Proc()
    {
        fInterval = 1; // 1s
        fQuit = false;
        start();
    }

    Proc::~Proc() {
        fQuit = true;
        wait(2000);
    }

    QVariantList Proc::getCPUUsage() {
        return fCPUUsage;
    }

    void Proc::run() Q_DECL_OVERRIDE {
        fCPUCount = getProcessorCount();
        fCPUTicks.resize(fCPUCount + 1); // room for "total"

        fCPUUsage.clear();
        for ( int i = 0; i <= fCPUCount; i++ ) {
            fCPUTicks[i] = 0.0f;
            fCPUUsage.append(0.0f);
        }

        while (!fQuit) {
            procCPUActivity();
            sleep(1);
        }
    }

    int Proc::getProcessorCount() {
        int count = 0;
        QFile cpuInfoFile("/proc/cpuinfo");
        if ( !cpuInfoFile.open(QIODevice::ReadOnly) ) {
            qCritical() << "Unable to open stat file /proc/stat\n";
            return 0;
        }

        QTextStream stream(&cpuInfoFile);
        QString line = stream.readLine(4096);
        while ( !line.isNull() ) {
            if ( line.contains("processor") ) {
                count++;
            }
            line = stream.readLine(4096);
        }

        return count;
    }

    unsigned long long Proc::parseCPUParts(QStringList &parts, int flags) {
        bool converted = true;
        unsigned long long result = 0;

        for ( int i = 0; i < CPU_PART_COUNT; i++ ) {
            int flag = CPU_PART_DEF[i];
            QString value = parts.at(i);
            if ( flag == flags ) {
                result += value.toULongLong(&converted);
            }
            if ( !converted ) {
                qCritical() << "Unable to convert " << value << " to unsigned long long\n";
                return 0;
            }
        }

        return result;
    }

    void Proc::procCPUActivity() {
        unsigned long long diffTicks;
        unsigned long long oldTicks;
        qreal usage;

        QFile statFile("/proc/stat");
        if ( !statFile.open(QIODevice::ReadOnly) ) {
            qCritical() << "Unable to open stat file /proc/stat\n";
            return;
        }

        QTextStream stream(&statFile);
        QString line;
        for ( int i = 0; i <= fCPUCount; i++ ) {
            line = stream.readLine(4096);
            if ( line.isNull() ) {
                qCritical() << "Error reading line for CPU: " << i << "\n";
                return;
            }

            QStringList parts = line.split(" ", QString::SkipEmptyParts);
            if ( parts.size() < 8 ) {
                qCritical() << "Not enough parts in CPU line: " << line << "\n";
                return;
            }

            QString type = parts.at(0);
            if ( !type.contains("cpu") ) {
                qWarning() << "Invalid CPU type indicator encountered: " << type;
                break;
            }

            oldTicks = fCPUTicks[i];
            fCPUTicks[i] = parseCPUParts(parts, CPU_FLAGS_ACTIVE);
            diffTicks = fCPUTicks[i] - oldTicks;
            usage = ((qreal)diffTicks / (qreal)(fInterval * 100)) * 100.0f;
            int iUsage = qRound(usage);

            if ( iUsage != fCPUUsage[i].toInt() ) {
                fCPUUsage[i] = iUsage;
            }
        }

        emit CPUUsageChanged(fCPUUsage);
    }

}
