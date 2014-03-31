#ifndef CPU_H
#define CPU_H

#include <QObject>
#include "types.h"

namespace Lighthouse {

    class CPU : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(IntList usage READ getUsage NOTIFY usageChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
    public:
        explicit CPU(QObject *parent = 0);

        IntList getUsage() const;
        int getSummaryValue();
    private:
        IntList fUsage;
    public slots:
        void setUsage(IntList usage);
    signals:
        void usageChanged();
        void summaryValueChanged();
    };

}

#endif // CPU_H
