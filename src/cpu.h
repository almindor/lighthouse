#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QVariantList>

namespace Lighthouse {

    class CPU : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QVariantList usage READ getUsage NOTIFY usageChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
    public:
        explicit CPU(QObject *parent = 0);

        QVariantList getUsage();
        int getSummaryValue();
    private:
        QVariantList fUsage;
    public slots:
        void setUsage(QVariantList usage);
    signals:
        void usageChanged();
        void summaryValueChanged();
    };

}

#endif // CPU_H
