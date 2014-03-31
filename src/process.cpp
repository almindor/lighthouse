#include "process.h"

namespace Lighthouse {

    // ProcessModel

    ProcessModel::ProcessModel(ProcList& procList, QObject *parent) : QAbstractListModel(parent), fProcList(procList) {

    }

    QHash<int, QByteArray> ProcessModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[DescrRole] = "descr";
        return roles;
    }

    Qt::ItemFlags ProcessModel::flags(const QModelIndex & index) const {
        return Qt::ItemIsEnabled;
    }

    QVariant ProcessModel::data(const QModelIndex & index, int role) const {
        const int row = index.row();
        if ( row >= 0 && row < fProcList.size() ) {
            return fProcList[row].toString();
        }

        return "Data[" + QString::number(index.row()) + "," + QString::number(index.column()) + "]: " + QString::number(role);
    }

    QVariant ProcessModel::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description";
    }

    int ProcessModel::rowCount(const QModelIndex & parent) const {
        return fProcList.size();
    }

    void ProcessModel::update() {
        emit dataChanged(createIndex(0, 0), createIndex(fProcList.size(), 0));
    }

    // Process

    Process::Process(QObject *parent) :
        QObject(parent), fProcList(), fModel(fProcList, parent)
    {
        fSortBy = 0;
    }

    int Process::getSummaryValue() const {
        return fProcList.size();
    }

    QString Process::getSortBy() const {
        switch ( fSortBy ) {
            case 0: return "CPU Usage";
            case 1: return "Memory Usage";
            case 2: return "Name";
        }

        return "Unknown";
    }

    ProcessModel& Process::getModel() {
        return fModel;
    }

    void Process::setProcList(ProcMap* procMap) {
        int oldCount = fProcList.size();
        fProcList = procMap->values();

        sort();
        emit procListChanged();
        if ( oldCount != fProcList.size() ) {
            emit summaryValueChanged();
        }
    }

    void Process::sort() {
        switch ( fSortBy ) {
            case 0: qSort(fProcList.begin(), fProcList.end(), CPUComparer()); break;
            case 1: qSort(fProcList.begin(), fProcList.end(), MemoryComparer()); break;
            case 2: qSort(fProcList.begin(), fProcList.end(), NameComparer()); break;
        }
        fModel.update();
    }

    void Process::nextSortBy() {
        fSortBy++;
        if ( fSortBy > 2 ) {
            fSortBy = 0;
        }

        sort();
        emit summaryValueChanged();
        emit sortByChanged();
    }

}
