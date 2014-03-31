#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    CPU::CPU(QObject *parent) :
        QObject(parent)
    {

    }

    IntList CPU::getUsage() const {
        return fUsage;
    }

    int CPU::getSummaryValue() {
        if ( fUsage.size() < 1 ) {
            return 0;
        }

        return fUsage[0];
    }

    void CPU::setUsage(IntList usage) {
        if ( usage.size() != fUsage.size() || fUsage != usage ) {
            fUsage = usage;
            emit usageChanged();
            emit summaryValueChanged();
        }
    }

}
