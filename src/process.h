#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVariantList>
#include <QAbstractListModel>
#include "types.h"

namespace Lighthouse {

    class ProcessModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum ProcessRoles {
            DescrRole = Qt::UserRole + 1
        };

        ProcessModel(ProcList& procList, QObject *parent = 0);

        QHash<int, QByteArray> roleNames() const;
        Qt::ItemFlags flags(const QModelIndex & index) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        void update();
    private:
        ProcList& fProcList;
    };

    class Process : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
        Q_PROPERTY(QString sortBy READ getSortBy NOTIFY sortByChanged)
    public:
        explicit Process(QObject *parent = 0);

        int getSummaryValue() const;
        QString getSortBy() const;
        ProcessModel& getModel();
        Q_INVOKABLE void nextSortBy();
    private:
        ProcList fProcList;
        int fSortBy;
        ProcessModel fModel;
        void sort();
    signals:
        void summaryValueChanged();
        void sortByChanged();
        void procListChanged();
    public slots:
        void setProcList(ProcMap* procMap);
    };

}

#endif // PROCESS_H
