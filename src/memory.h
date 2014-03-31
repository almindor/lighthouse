#ifndef MEMORY_H
#define MEMORY_H

#include <QObject>

namespace Lighthouse {

    class Memory : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int free READ getFree NOTIFY freeChanged)
        Q_PROPERTY(int total READ getTotal NOTIFY totalChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
    public:
        explicit Memory(QObject *parent = 0);

        int getFree() const;
        int getTotal() const;
        int getSummaryValue() const;
    private:
        int fFree;
        int fTotal;
    signals:
        void freeChanged(int free);
        void totalChanged(int total);
        void summaryValueChanged(int value);
    public slots:
        void setMemory(int total, int free);
    };

}

#endif // MEMORY_H
