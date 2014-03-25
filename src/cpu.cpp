#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    const int USAGE_LEVEL_COUNT = 5;
    const QString USAGE_LEVELS[USAGE_LEVEL_COUNT] = { "Minimal", "Low", "Medium", "High", "Full" };

    QVariantList CPU::getUsage() {
        return fUsage;
    }

    QString CPU::getCoverInfo() {
        return USAGE_LEVELS[fUsageLevel];
    }

    bool CPU::getPageActive() {
        return fPageActive;
    }

    void CPU::setPageActive(bool active) {
        fPageActive = active;
    }

    void CPU::setUsage(QVariantList usage) {
        fUsage = usage;
        if ( fPageActive ) {
            emit usageChanged(usage);
        }

        if ( fUsage.size() < 1 ) {
            return;
        }

        int usageLevel = fUsage[0].toInt() / USAGE_LEVEL_COUNT;
        if ( usageLevel < 0 ) {
            usageLevel = 0;
        } else if ( usageLevel >= USAGE_LEVEL_COUNT ) {
            usageLevel = USAGE_LEVEL_COUNT - 1;
        }
        if ( usageLevel != fUsageLevel ) {
            fUsageLevel = usageLevel;
            emit coverInfoChanged(getCoverInfo());
        }
    }

}
