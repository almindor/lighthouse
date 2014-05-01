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

    Process::Process(QObject *parent) : QAbstractListModel(parent), fProcList(), fAppList() {
        fUID = getuid();
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
        const ProcList& list = getList();
        const int row = index.row();
        if ( row >= 0 && row < list.size() ) {
            switch ( role ) {
                case PIDRole: return list.at(row).getPID();
                case NameRole: return list.at(row).getName();
                case CPUUsageRole: return list.at(row).getCPUUsage();
                case MemoryUsageRole: return list.at(row).getMemoryUsage();
                case SelectedRole: return (fSelectedPID == list.at(row).getPID());
            }
        }

        switch ( role ) {
            case PIDRole: return 0;
            case NameRole: return "Unknown";
            case CPUUsageRole: return 0;
            case MemoryUsageRole: return 0;
            case SelectedRole: return false;
        }

        return QVariant();
    }

    QVariant Process::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description: " + section + orientation + role;
    }

    int Process::rowCount(const QModelIndex & parent) const {
        return getList().size();
    }

    int Process::getSummaryValue() const {
        return fProcCount;
    }

    void Process::setProcesses(ProcMap* procMap) {
        if ( fSelectedPID > 0 ) {
            return; // don't update if we're selecting to kill
        }

        const ProcList sourceList = procMap->values();
        ProcList procList;
        ProcList appList;
        foreach ( ProcInfo item, sourceList ) {
            if ( item.isApplication() ) {
                appList.append(item);
            }
            procList.append(item);
        }

        updateList(fProcList, procList);
        updateList(fAppList, appList);

        if ( fProcCount != fProcList.size() ) {
            fProcCount = fProcList.size();
            emit summaryValueChanged();
        }
    }

    void Process::setProcessCount(int count) {
        if ( count != fProcCount ) {
            fProcCount = count;
            emit summaryValueChanged();
        }
    }

    void Process::updateList(ProcList& dest, ProcList& source) {
        if ( dest == getList() ) {
            const int oldSize = dest.size();
            const int sourceSize = source.size();

            if ( oldSize > sourceSize ) { // removed process
                beginRemoveRows(QModelIndex(), sourceSize, oldSize - 1);
                dest = source;
                //qDebug() << "Removed process at index range: " << sourceSize << " - " << (oldSize - 1) << "\n";
                endRemoveRows();
            } else if ( oldSize < sourceSize) { // added process
                beginInsertRows(QModelIndex(), oldSize, sourceSize - 1);
                dest = source;
                //qDebug() << "Added process at index range: " << oldSize << " - " << (sourceSize - 1) << "\n";
                endInsertRows();
            } else {
                dest = source;
            }
            sort(dest);

            emit dataChanged(createIndex(0, 0), createIndex(dest.size(), 0));
        } else {
            dest = source;
        }
    }

    void Process::sort(ProcList& list) {
        switch ( fSortBy ) {
            case 0: qSort(list.begin(), list.end(), CPUComparer()); break;
            case 1: qSort(list.begin(), list.end(), MemoryComparer()); break;
            case 2: qSort(list.begin(), list.end(), NameComparer()); break;
        }
    }

    const ProcList& Process::getList() const {
        return fApplicationsOnly ? fAppList : fProcList;
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

            ProcList& list = fApplicationsOnly ? fAppList : fProcList;
            sort(list);
            emit dataChanged(createIndex(0, 0), createIndex(list.size(), 0));
        }
    }

    void Process::nextApplicationsOnly() {
        beginResetModel();
        fApplicationsOnly = !fApplicationsOnly;
        sort(fApplicationsOnly ? fAppList : fProcList);
        endResetModel();
        emit applicationsOnlyChanged();
    }

    void Process::selectPID(int pid) {
        if ( fSelectedPID != pid ) {
            fSelectedPID = pid;
            emit selectedPIDChanged();

            const ProcList& list = getList();
            const int size = list.size();

            for ( int i = 0; i < size; i++ ) {
                if ( list[i].getPID() == pid ) {
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
