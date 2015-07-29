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
#include <QSettings>
#include "types.h"

namespace Lighthouse {

    class Process : public QAbstractListModel
    {
        Q_OBJECT
        // processes
        Q_PROPERTY(int selectedPID READ getSelectedPID WRITE selectPID NOTIFY selectedPIDChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
        Q_PROPERTY(int sortBy READ getSortBy NOTIFY sortByChanged)
        Q_PROPERTY(bool applicationsOnly READ getApplicationsOnly NOTIFY applicationsOnlyChanged)
        // details
        Q_PROPERTY(int selectedCPUUsage READ getSelectedCPUUsage NOTIFY selectedChanged)
        Q_PROPERTY(int selectedMemoryUsage READ getSelectedMemoryUsage NOTIFY selectedChanged)
        Q_PROPERTY(const QString selectedName READ getSelectedName NOTIFY selectedChanged)
    public:
        enum ProcessRoles {
            PIDRole = Qt::UserRole + 1,
            NameRole,
            CPUUsageRole,
            MemoryUsageRole
        };

        explicit Process(QObject *parent = 0);

        QHash<int, QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex & index) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        int getSortBy() const;
        int getSummaryValue() const;
        bool getApplicationsOnly() const;
        int getSelectedPID() const;
        void selectPID(int pid);
        Q_INVOKABLE bool isKillable() const;
        Q_INVOKABLE void setSortBy(int sb);
        Q_INVOKABLE void nextApplicationsOnly();
        Q_INVOKABLE int killSelected();
    private:
        uid_t fUID;
        int fSortBy;
        int fSelectedPID;
        const ProcMap* fProcMap;
        PIDList fProcKeys;
        PIDList fAppKeys;
        bool fApplicationsOnly;
        int fPageStatus;
        bool fApplicationActive;
        int fProcCount;
        CPUComparer fCPUComparer;
        MemoryComparer fMemoryComparer;
        NameComparer fNameComparer;
        QSettings fSettings;

        void removeKeys(const PIDList &deletes, PIDList& list, bool really);
        void appendKeys(const PIDList &adds, PIDList& list, bool really, bool appsOnly);
        void sort(PIDList& list);
        const PIDList& getKeys() const;
        const BaseComparer* getComparer() const;
        void beginRR(bool really, int first, int last);
        void endRR(bool really);
        void beginIR(bool really, int first, int last);
        void endIR(bool really);

        const QString getSelectedName() const;
        int getSelectedCPUUsage() const;
        int getSelectedMemoryUsage() const;
    signals:
        void summaryValueChanged();
        void sortByChanged();
        void selectedPIDChanged();
        void applicationsOnlyChanged();
        void selectedChanged();
    public slots:
        void setProcesses(const ProcMap* procMap, const PIDList& adds, const PIDList& deletes);
        void setProcessCount(int count);
    };

}

#endif // PROCESS_H
