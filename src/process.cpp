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

    // ProcessModel

    Process::Process(QObject *parent) : QAbstractListModel(parent), fProcKeys(), fAppKeys(),
                     fCPUComparer(), fMemoryComparer(), fNameComparer() {
        fUID = getuid();
        fProcMap = 0;
        fSortBy = 0;
        fSelectedPID = 0;
        fApplicationsOnly = false;
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
        roles[SelectedRole] = "selected";
        return roles;
    }

    Qt::ItemFlags Process::flags(const QModelIndex & index) const {
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
                case SelectedRole: return (fSelectedPID == fProcMap->value(key).getPID());
            }
        }

        switch ( role ) {
            case PIDRole: return 0;
            case NameRole: return "Unknown";
            case CPUUsageRole: return 0;
            case MemoryUsageRole: return 0;
            case SelectedRole: return false;
        }

        return 0;
    }

    QVariant Process::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description: " + section + orientation + role;
    }

    int Process::rowCount(const QModelIndex & parent) const {
        return getKeys().size();
    }

    int Process::getSummaryValue() const {
        return fProcCount;
    }

    void Process::removeKeys(const PIDList &deletes, PIDList& list, bool really, bool appsOnly) {
        if ( deletes.size() > 0 ) {
            foreach ( pid_t pid, deletes ) {
                const ProcInfo info = fProcMap->value(pid);
                if ( appsOnly && !info.isApplication() ) {
                    continue;
                }
                for ( int i = 0; i < list.size(); i++ ) {
                    const pid_t key = list.at(i);
                    if ( key == pid ) {
                        beginRR(really, i, i);
                        list.removeAt(i);
                        endRR(really);
                        break;
                    }
                }
            }
        }
    }

    void Process::appendKeys(const PIDList &adds, PIDList& list, bool really, bool appsOnly) {
        if ( adds.size() > 0 ) {
            const BaseComparer* compare = getComparer();
            foreach ( pid_t pid, adds ) {
                const ProcInfo info = fProcMap->value(pid);
                if ( appsOnly && !info.isApplication() ) {
                    continue;
                }
                bool done = false;
                int i;
                for ( i = 0; i < list.size(); i++ ) {
                    const pid_t key = list.at(i);
                    if ( compare->operator ()(key, pid) ) {
                        beginIR(really, i, i);
                        list.insert(i, pid);
                        endIR(really);
                        done = true;
                        break;
                    }
                }

                if ( !done ) {
                    beginIR(really, i, i);
                    list.insert(i, pid);
                    endIR(really);
                }
            }
        }

    }

    void Process::setProcesses(const ProcMap* procMap, const PIDList& adds, const PIDList& deletes) {
        if ( fSelectedPID > 0 ) {
            return; // don't update if we're selecting to kill
        }

        fProcMap = procMap;
        fCPUComparer.setProcMap(fProcMap);
        fMemoryComparer.setProcMap(fProcMap);
        fNameComparer.setProcMap(fProcMap);
        qDebug() << "Original ADDR: " << (unsigned long)(&procMap) << " NEW: " << (unsigned long)fProcMap;

        removeKeys(deletes, fProcKeys, !fApplicationsOnly, false);
        removeKeys(deletes, fAppKeys, fApplicationsOnly, true);
        appendKeys(adds, fProcKeys, !fApplicationsOnly, false);
        appendKeys(adds, fAppKeys, fApplicationsOnly, true);

        sort(fProcKeys);
        sort(fAppKeys);
        emit dataChanged(createIndex(0, 0), createIndex(getKeys().size(), 0));

        if ( fProcCount != getKeys().size() ) {
            fProcCount = getKeys().size();
            emit summaryValueChanged();
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
        return fApplicationsOnly ? fAppKeys : fProcKeys;
    }

    int Process::getSortBy() const {
        return fSortBy;
    }

    bool Process::isKillable(int pid) const {
        struct stat fStat;
        QString path = "/proc/" + QString::number(pid) + "/stat";
        if ( stat(path.toLocal8Bit().data(), &fStat) != 0 ) {
            qCritical() << "Unable to stat file: " << path << ": " << strerror(errno) << "\n";
            return false;
        }

        if ( fStat.st_uid == fUID ) {
            return true;
        }

        return false;
    }

    int Process::getSelectedPID() const {
        return fSelectedPID;
    }

    bool Process::getApplicationsOnly() const {
        return fApplicationsOnly;
    }

    void Process::setSortBy(int sb) {
        if ( sb != fSortBy && sb >= 0 && sb < 3 ) {
            fSortBy = sb;
            emit sortByChanged();

            PIDList& keys = fApplicationsOnly ? fAppKeys : fProcKeys;
            sort(keys);
            emit dataChanged(createIndex(0, 0), createIndex(keys.size(), 0));
        }
    }

    void Process::nextApplicationsOnly() {
        beginResetModel();
        fApplicationsOnly = !fApplicationsOnly;
        endResetModel();
        emit applicationsOnlyChanged();
    }

    void Process::selectPID(int pid) {
        if ( fSelectedPID != pid ) {
            fSelectedPID = pid;
            emit selectedPIDChanged();

            const PIDList& keys = getKeys();
            const int size = keys.size();

            for ( int i = 0; i < size; i++ ) {
                if ( fProcMap->value(keys.at(i)).getPID() == pid ) {
                    emit dataChanged(createIndex(i, 0), createIndex(i, 0));
                    return;
                }
            }
        }
    }

    void Process::killSelectedProcess() {
        if ( fSelectedPID > 0 ) {
            if ( kill(fSelectedPID, SIGKILL) != 0 ) {
                qCritical() << "Unable to kill process: " << fSelectedPID << " error: " << strerror(errno) << "\n";
            }
        }
    }

}
