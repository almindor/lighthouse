#include "process.h"
#include <QDebug>

namespace Lighthouse {

    // ProcessModel

    Process::Process(QObject *parent) : QAbstractListModel(parent), fProcList() {
        fSortBy = 0;
    }

    QHash<int, QByteArray> Process::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[DescrRole] = "descr";
        return roles;
    }

    Qt::ItemFlags Process::flags(const QModelIndex & index) const {
        return Qt::ItemIsEnabled;
    }

    QVariant Process::data(const QModelIndex & index, int role) const {
        const int row = index.row();
        if ( row >= 0 && row < fProcList.size() ) {
            return fProcList[row].toString();
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

            if ( startDone && endDone || start > end ) {
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

    void Process::nextSortBy() {
        fSortBy++;
        if ( fSortBy > 2 ) {
            fSortBy = 0;
        }
        emit sortByChanged();

        sort(fProcList);
        emit dataChanged(createIndex(0, 0), createIndex(fProcList.size(), 0));
    }

}
