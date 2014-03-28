#include "memory.h"
#include <QDebug>

namespace Lighthouse {

    Memory::Memory(QObject *parent) :
        QObject(parent)
    {
        fTotal = 0;
        fFree = 0;
    }

    int Memory::getFree() {
        return fFree;
    }

    int Memory::getTotal() {
        return fTotal;
    }

    int Memory::getSummaryValue() {
        return 100 - qRound((qreal)fFree / (qreal)fTotal * 100.0f);
    }

    void Memory::setMemory(int total, int free) {
        int changed = 0;
        if ( fTotal != total ) {
            fTotal = total;
            emit totalChanged(total);
            changed++;
        }

        if ( fFree != free ) {
            fFree = free;
            emit freeChanged(free);
            changed++;
        }

        if ( changed > 0 ) {
            emit summaryValueChanged(getSummaryValue());
        }
    }

}
