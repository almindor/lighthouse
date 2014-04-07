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
#include <QDebug>

namespace Lighthouse {

    // ProcessModel

    Process::Process(QObject *parent) : QAbstractListModel(parent), fProcList() {
        fUID = getuid();
        fSortBy = 0;
        fSelectedPID = 0;
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
        const int row = index.row();
        if ( row >= 0 && row < fProcList.size() ) {
            switch ( role ) {
                case PIDRole: return fProcList[row].getPID();
                case NameRole: return fProcList[row].getName();
                case CPUUsageRole: return fProcList[row].getCPUUsage();
                case MemoryUsageRole: return fProcList[row].getMemoryUsage();
                case SelectedRole: return (fSelectedPID == fProcList[row].getPID());
            }
        }

        return "Data[" + QString::number(index.row()) + "," + QString::number(index.column()) + "]: " + QString::number(role);
    }

    QVariant Process::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description: " + section + orientation + role;
    }

    int Process::rowCount(const QModelIndex & parent) const {
        return fProcList.size();
    }

    int Process::getSummaryValue() const {
        return fProcList.size();
    }

    void Process::setProcList(ProcMap* procMap) {
        if ( fSelectedPID > 0 ) {
            return; // don't update if we're selecting to kill
        }

        int oldSize = fProcList.size();
        ProcList procList = procMap->values();
        sort(procList);
        int startRow;
        int endRow;
        diffProcLists(fProcList, procList, startRow, endRow);
        fProcList = procList;
        //qDebug() << "diff from " << startRow << " to " << endRow << "\n";

        if ( startRow <= endRow ) {
            emit dataChanged(createIndex(startRow, 0), createIndex(endRow, 0));
        }

        if ( oldSize != fProcList.size() ) {
            emit summaryValueChanged();
        }
    }

    void Process::sort(ProcList& list) {
        switch ( fSortBy ) {
            case 0: qSort(list.begin(), list.end(), CPUComparer()); break;
            case 1: qSort(list.begin(), list.end(), MemoryComparer()); break;
            case 2: qSort(list.begin(), list.end(), NameComparer()); break;
        }
    }

    void Process::diffProcLists(const ProcList& oldList, const ProcList& newList, int& start, int& end) const {
        const int newSize = newList.size();
        const int oldSize = oldList.size();
        const int size = newSize <= oldSize ? newSize : oldSize;
        bool endDone = false;
        bool startDone = false;
        int x;
        start = 0;
        end = oldList.size() - 1;

        for ( int i = 0; i < size; i++ ) {
            x = size - i - 1;
            if ( !startDone && oldList.at(i) == newList.at(i) ) {
                start++;
            } else {
                startDone = true;
            }

            if ( !endDone && oldList.at(x) == newList.at(x) ) {
                end--;
            } else {
                endDone = true;
            }

            if ( (startDone && endDone) || start > end ) {
                return;
            }
        }
    }

    QString Process::getSortBy() const {
        switch ( fSortBy ) {
            case 0: return "CPU Usage";
            case 1: return "Memory Usage";
            case 2: return "Name";
        }

        return "Unknown";
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

    void Process::nextSortBy() {
        fSortBy++;
        if ( fSortBy > 2 ) {
            fSortBy = 0;
        }
        emit sortByChanged();

        sort(fProcList);
        emit dataChanged(createIndex(0, 0), createIndex(fProcList.size(), 0));
    }

    void Process::selectPID(int pid) {
        if ( fSelectedPID != pid ) {
            fSelectedPID = pid;
            emit selectedPIDChanged();

            const int size = fProcList.size();

            for ( int i = 0; i < size; i++ ) {
                if ( fProcList[i].getPID() == pid ) {
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
