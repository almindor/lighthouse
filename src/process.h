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
        Q_PROPERTY(QString sortBy READ getSortBy NOTIFY sortByChanged)
    public:
        enum ProcessRoles {
            DescrRole = Qt::UserRole + 1
        };

        Process(QObject *parent = 0);

        QHash<int, QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex & index) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        Q_INVOKABLE void nextSortBy();
        QString getSortBy() const;
        int getSummaryValue() const;
    private:
        void sort(ProcList& list);
        void diffProcLists(const ProcList& oldList, const ProcList& newList, int& start, int& end) const;
        int fSortBy;
        ProcList fProcList;
    signals:
        void summaryValueChanged();
        void sortByChanged();
    public slots:
        void setProcList(ProcMap* procMap);
    };

}

#endif // PROCESS_H
