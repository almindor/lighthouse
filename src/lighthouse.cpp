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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "sailfishapp.h"
#include <QObject>
#include <QString>
#include <QQuickView>
#include <QQmlContext>
#include <QGuiApplication>
#include <QFileSystemWatcher>
#include "types.h"
#include "languages.h"
#include "monitor.h"
#include "cpu.h"
#include "memory.h"
#include "zram.h"
#include "process.h"
#include "battery.h"

using namespace Lighthouse;

int main(int argc, char *argv[])
{
    int result = 0;
    qRegisterMetaType< IntList >( "IntList" );
    qRegisterMetaType< PIDList >( "PIDList" );
    qRegisterMetaType< ProcMap >( "ProcMap" );

    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setOrganizationName(QStringLiteral("ltd.bitsmart"));
    app->setApplicationName(QStringLiteral("Lighthouse"));

    Languages languages(app);
    Monitor monitor;
    CPU cpu;
    Process process;
    Memory memory;
    Zram zram;
    Battery battery;
    QFileSystemWatcher appsWatch;
    appsWatch.addPath("/usr/share/applications");

    QQuickView *view = SailfishApp::createView();

    QObject::connect(&monitor, &Monitor::CPUUsageChanged,
                     &cpu, &CPU::setUsage);
    QObject::connect(&monitor, &Monitor::temperatureChanged,
                     &cpu, &CPU::setTemperature);
    QObject::connect(&monitor, &Monitor::memoryChanged,
                     &memory, &Memory::setMemory);
    QObject::connect(&monitor, &Monitor::zramChanged,
                     &zram, &Zram::onZramChanged);
    QObject::connect(&monitor, &Monitor::processChanged,
                     &process, &Process::setProcesses);
    QObject::connect(&monitor, &Monitor::processCountChanged,
                     &process, &Process::setProcessCount);
    QObject::connect(&monitor, &Monitor::batteryHealthChanged,
                     &battery, &Battery::setHealth);
    QObject::connect(&monitor, &Monitor::batteryTechnologyChanged,
                     &battery, &Battery::setTechnology);
    QObject::connect(&monitor, &Monitor::batteryLevelChanged,
                     &battery, &Battery::setLevel);
    QObject::connect(&monitor, &Monitor::batteryStatusChanged,
                     &battery, &Battery::setStatus);
    QObject::connect(&appsWatch, &QFileSystemWatcher::directoryChanged,
                     &monitor, &Monitor::updateApplicationMap);

    QString qml = QString("qml/%1.qml").arg(ANAME);
    view->rootContext()->setContextProperty("languages", &languages);
    view->rootContext()->setContextProperty("cpu", &cpu);
    view->rootContext()->setContextProperty("memory", &memory);
    view->rootContext()->setContextProperty("zram", &zram);
    view->rootContext()->setContextProperty("monitor", &monitor);
    view->rootContext()->setContextProperty("process", &process);
    view->rootContext()->setContextProperty("battery", &battery);
    view->setSource(SailfishApp::pathTo(qml));
    view->show();

    monitor.start(); // start thread after all receivers are connected

    result = app->exec();

    delete view;
    delete app;

    return result;
}

