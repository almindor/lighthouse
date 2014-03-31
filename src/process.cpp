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
#include <QDebug>

namespace Lighthouse {

    // ProcessModel

    Process::Process(QObject *parent) : QAbstractListModel(parent), fProcList() {
        fSortBy = 0;
    }

    QHash<int, QByteArray> Process::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        roles[CPUUsageRole] = "cpuUsage";
        roles[MemoryUsageRole] = "memoryUsage";
        return roles;
    }

    Qt::ItemFlags Process::flags(const QModelIndex & index) const {
        return Qt::ItemIsEnabled;
    }

    QVariant Process::data(const QModelIndex & index, int role) const {
        const int row = index.row();
        if ( row >= 0 && row < fProcList.size() ) {
            switch ( role ) {
                case NameRole: return fProcList[row].getName();
                case CPUUsageRole: return fProcList[row].getCPUUsage();
                case MemoryUsageRole: return fProcList[row].getMemoryUsage();
            }
        }

        return "Data[" + QString::number(index.row()) + "," + QString::number(index.column()) + "]: " + QString::number(role);
    }

    QVariant Process::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description";
    }

    int Process::rowCount(const QModelIndex & parent) const {
        return fProcList.size();
    }

    int Process::getSummaryValue() const {
        return fProcList.size();
    }

    void Process::setProcList(ProcMap* procMap) {
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
            case 1: qSort(list.begin(), list.end(), NameComparer()); break;
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
            case 1: return "Name";
        }

        return "Unknown";
    }

    void Process::nextSortBy() {
        fSortBy++;
        if ( fSortBy > 1 ) {
            fSortBy = 0;
        }
        emit sortByChanged();

        sort(fProcList);
        emit dataChanged(createIndex(0, 0), createIndex(fProcList.size(), 0));
    }

}
