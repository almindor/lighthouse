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

#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVariantList>
#include <QAbstractListModel>
#include "types.h"

namespace Lighthouse {

    class Process : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
        Q_PROPERTY(int sortBy READ getSortBy NOTIFY sortByChanged)
        Q_PROPERTY(int selectedPID READ getSelectedPID NOTIFY selectedPIDChanged)
        Q_PROPERTY(bool applicationsOnly READ getApplicationsOnly NOTIFY applicationsOnlyChanged)
    public:
        enum ProcessRoles {
            PIDRole = Qt::UserRole + 1,
            NameRole,
            CPUUsageRole,
            MemoryUsageRole,
            SelectedRole
        };

        explicit Process(QObject *parent = 0);

        QHash<int, QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex & index) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        int getSortBy() const;
        int getSummaryValue() const;
        int getSelectedPID() const;
        bool getApplicationsOnly() const;
        Q_INVOKABLE bool isKillable(int pid) const;
        Q_INVOKABLE void setSortBy(int sb);
        Q_INVOKABLE void nextApplicationsOnly();
        Q_INVOKABLE void selectPID(int pid);
        Q_INVOKABLE void killSelectedProcess();
    private:
        uid_t fUID;
        int fSortBy;
        int fSelectedPID;
        ProcMap fProcMap;
        PIDList fProcKeys;
        PIDList fAppKeys;
        bool fApplicationsOnly;
        int fPageStatus;
        bool fApplicationActive;
        int fProcCount;
        CPUComparer fCPUComparer;
        MemoryComparer fMemoryComparer;
        NameComparer fNameComparer;

        void sort(PIDList& list);
        const PIDList& getKeys() const;
        const BaseComparer* getComparer() const;
    signals:
        void summaryValueChanged();
        void sortByChanged();
        void selectedPIDChanged();
        void applicationsOnlyChanged();
    public slots:
        void setProcesses(const ProcMap& procMap, const PIDList& adds, const PIDList& deletes);
        void setProcessCount(int count);
    };

}

#endif // PROCESS_H
