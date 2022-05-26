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

#include "process.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <QDir>
#include <QSettings>
#include <QDebug>

namespace Lighthouse {

    const int KEY_APPS = 0;
    const int KEY_PROC_NOSYS = 1;
    const int KEY_PROC_ALL = 2;

    // ProcessModel

    Process::Process(QObject *parent) : QAbstractListModel(parent), fPidKeys(),
                     fCPUComparer(), fMemoryComparer(), fNameComparer(), fSettings() {
        fUID = getuid();
        fProcMap = 0;
        fSortBy = 0;
        fSelectedPID = 0;
        fSelectedTick = 0;
        fPidKey = fSettings.value("proc/pidkey", -1).toInt();
        if ( fPidKey < 0 ) {
            // legacy
            bool appsOnly = fSettings.value("proc/apponly", false).toBool();
            fPidKey = appsOnly ? KEY_APPS : KEY_PROC_NOSYS;
        }
        fPageStatus = 0;
        fApplicationActive = false;
        fProcCount = 0;
    }

    QHash<int, QByteArray> Process::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[PIDRole] = "processID";
        roles[NameRole] = "name";
        roles[CPUUsageRole] = "cpuUsage";
        roles[MemoryUsageRole] = "memoryUsage";
        return roles;
    }

    Qt::ItemFlags Process::flags(const QModelIndex & index __attribute__ ((unused)) ) const {
        return Qt::ItemIsEnabled;
    }

    QVariant Process::data(const QModelIndex & index, int role) const {
        const PIDList& keys = getKeys();
        const int row = index.row();
        const pid_t key = keys.at(row);
        if ( fProcMap->contains(key) ) {
            switch ( role ) {
                case PIDRole: return fProcMap->value(key).getPID();
                case NameRole: return fProcMap->value(key).getName();
                case CPUUsageRole: return fProcMap->value(key).getCPUUsage();
                case MemoryUsageRole: return fProcMap->value(key).getMemoryUsage();
            }
        }

        switch ( role ) {
            case PIDRole: return 0;
            case NameRole: return tr("Unknown", "Process name");
            case CPUUsageRole: return 0;
            case MemoryUsageRole: return 0;
        }

        return 0;
    }

    QVariant Process::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description: " + QString::number(section) + orientation + QString::number(role);
    }

    int Process::rowCount(const QModelIndex & parent __attribute__ ((unused)) ) const {
        return getKeys().size();
    }

    int Process::getSummaryValue() const {
        return fProcCount;
    }

    void Process::removeKeys(const PIDList &deletes, PIDList& list, int pidKey) {
        if ( deletes.size() > 0 ) {
            foreach ( pid_t pid, deletes ) {
                const ProcInfo info = fProcMap->value(pid);
                for ( int i = 0; i < list.size(); i++ ) {
                    const pid_t key = list.at(i);
                    if ( key == pid ) {
                        beginRR(pidKey == fPidKey, i, i);
                        list.removeAt(i);
                        endRR(pidKey == fPidKey);
                        break;
                    }
                }
            }
        }
    }

    void Process::appendKeys(const PIDList &adds, PIDList& list, int pidKey) {
        if ( adds.size() > 0 ) {
            const BaseComparer* compare = getComparer();
            foreach ( pid_t pid, adds ) {
                const ProcInfo info = fProcMap->value(pid);
                // skip non apps for app map
                if ( pidKey == KEY_APPS && !info.isApplication() ) {
                    continue;
                }
                // skip system process for non system map
                if ( pidKey == KEY_PROC_NOSYS && info.isSystemProc() ) {
                    continue;
                }

                bool done = false;
                int i;
                for ( i = 0; i < list.size(); i++ ) {
                    const pid_t key = list.at(i);
                    if ( compare->operator ()(key, pid) ) {
                        beginIR(pidKey == fPidKey, i, i);
                        list.insert(i, pid);
                        endIR(pidKey == fPidKey);
                        done = true;
                        break;
                    }
                }

                if ( !done ) {
                    beginIR(pidKey == fPidKey, i, i);
                    list.insert(i, pid);
                    endIR(pidKey == fPidKey);
                }
            }
        }

    }

    void Process::setProcesses(const ProcMap* procMap, const PIDList& adds, const PIDList& deletes) {
        //qDebug() << "setProcesses: " << procMap->size() << " Adds: " << adds.size() << "Deletes: " << deletes.size() << "\n";
        fProcMap = procMap;
        fCPUComparer.setProcMap(fProcMap);
        fMemoryComparer.setProcMap(fProcMap);
        fNameComparer.setProcMap(fProcMap);

        PIDList& procKeys = fPidKeys[KEY_PROC_ALL];
        PIDList& procKeysNoSys = fPidKeys[KEY_PROC_NOSYS];
        PIDList& appKeys = fPidKeys[KEY_APPS];

        removeKeys(deletes, procKeys, KEY_PROC_ALL);
        removeKeys(deletes, procKeysNoSys, KEY_PROC_NOSYS);
        removeKeys(deletes, appKeys, KEY_APPS);
        appendKeys(adds, procKeys, KEY_PROC_ALL);
        appendKeys(adds, procKeysNoSys, KEY_PROC_NOSYS);
        appendKeys(adds, appKeys, KEY_APPS);

        sort(procKeys);
        sort(appKeys);
        emit dataChanged(createIndex(0, 0), createIndex(getKeys().size(), 0));

        const int keySize = procKeys.size();
        if ( fProcCount != keySize ) {
            fProcCount = keySize;
            emit summaryValueChanged();
        }

        if ( fSelectedPID > 0 ) {
            fSelectedTick++;
            emit selectedChanged();
        }
    }

    void Process::setProcessCount(int count) {
        if ( count != fProcCount ) {
            fProcCount = count;
            emit summaryValueChanged();
        }
    }

    void Process::sort(PIDList& list) {
        switch ( fSortBy ) {
            case 0: qSort(list.begin(), list.end(), fCPUComparer); break;
            case 1: qSort(list.begin(), list.end(), fMemoryComparer); break;
            case 2: qSort(list.begin(), list.end(), fNameComparer); break;
        }
    }

    const BaseComparer* Process::getComparer() const {
        switch ( fSortBy ) {
            case 0: return &fCPUComparer; break;
            case 1: return &fMemoryComparer; break;
            case 2: return &fNameComparer; break;
        }

        return &fCPUComparer;
    }

    void Process::beginRR(bool really, int first, int last) {
        if ( really ) {
            beginRemoveRows(QModelIndex(), first, last);
        }
    }

    void Process::endRR(bool really) {
        if ( really ) {
            endRemoveRows();
        }
    }

    void Process::beginIR(bool really, int first, int last) {
        if ( really ) {
            beginInsertRows(QModelIndex(), first, last);
        }
    }

    void Process::endIR(bool really) {
        if ( really ) {
            endInsertRows();
        }
    }

    const PIDList& Process::getKeys() const {
        const PIDList& result = fPidKeys[fPidKey];

        return result;
    }

    int Process::getSortBy() const {
        return fSortBy;
    }

    bool Process::isKillable() const {
        struct stat fStat;
        QString path = "/proc/" + QString::number(fSelectedPID) + "/stat";
        if ( stat(path.toLocal8Bit().data(), &fStat) != 0 ) {
            qCritical() << "Unable to stat file: " << path << ": " << strerror(errno) << "\n";
            return false;
        }

        if ( fStat.st_uid == fUID ) {
            return true;
        }

        return false;
    }

    int Process::getPidKey() const {
        return fPidKey;
    }

    void Process::setSortBy(int sb) {
        if ( sb != fSortBy && sb >= 0 && sb < 3 ) {
            fSortBy = sb;
            emit sortByChanged();

            PIDList& keys = fPidKeys[fPidKey];
            sort(keys);
            emit dataChanged(createIndex(0, 0), createIndex(keys.size(), 0));
        }
    }

    void Process::setPidKey(int pidKey) {
        if ( pidKey == fPidKey ) {
            return;
        }

        beginResetModel();
        fPidKey = pidKey;
        fSettings.setValue("proc/pidkey", fPidKey);
        endResetModel();
        emit pidKeyChanged();
    }

    int Process::getSelectedPID() const {
        return fSelectedPID;
    }

    void Process::selectPID(int pid) {
        fSelectedPID = pid;
        emit selectedPIDChanged();
    }

    int Process::killSelected() {
        int result = -1;
        if ( fSelectedPID > 0 ) {
            result = kill(fSelectedPID, SIGKILL);

            if ( result != 0 ) {
                qCritical() << "Unable to kill process: " << fSelectedPID << " error: " << strerror(errno) << "\n";
            }
        }

        return result;
    }

    const QString Process::getSelectedName() const {
        return fSelectedPID > 0 ? fProcMap->value(fSelectedPID).getName() : "Invalid process";
    }

    const QString Process::getSelectedProcName() const
    {
        return fSelectedPID > 0 ? fProcMap->value(fSelectedPID).getProcName() : "Invalid process";
    }

    int Process::getSelectedCPUUsage() const {
        return fSelectedPID > 0 ? fProcMap->value(fSelectedPID).getCPUUsage() : 0;
    }

    int Process::getSelectedMemoryUsage() const {
        return fSelectedPID > 0 ? fProcMap->value(fSelectedPID).getMemoryUsage() : 0;
    }

    int Process::getSelectedTick() const {
        return fSelectedTick;
    }

}
