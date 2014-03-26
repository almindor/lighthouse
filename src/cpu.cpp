#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    CPU::CPU(QObject *parent) :
        QObject(parent)
    {

    }

    QVariantList CPU::getUsage() {
        return fUsage;
    }

    int CPU::getSummaryValue() {
        if ( fUsage.size() < 1 ) {
            return 0;
        }

        return fUsage[0].toInt();
    }

    void CPU::setUsage(QVariantList usage) {
        fUsage = usage;
        emit usageChanged();
        emit summaryValueChanged();

        if ( fUsage.size() < 1 ) {
            return;
        }
    }

}
