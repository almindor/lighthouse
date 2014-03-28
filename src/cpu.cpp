#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    CPU::CPU(QObject *parent) :
        QObject(parent)
    {

    }

    IntList CPU::getUsage() {
        return fUsage;
    }

    int CPU::getSummaryValue() {
        if ( fUsage.size() < 1 ) {
            return 0;
        }

        return fUsage[0];
    }

    void CPU::setUsage(IntList usage) {
        fUsage = usage;
        emit usageChanged();
        emit summaryValueChanged();
    }

}
