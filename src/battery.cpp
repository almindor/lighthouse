#include "battery.h"
#include <QtDebug>

namespace Lighthouse {
    Battery::Battery(QObject *parent) :
        QObject(parent)
    {
            fLevel = 0;
            fStatus = "Unavailable";
            fHealth = "Unavailable";
            fTechnology = "Unavailable";
    }
    
    void Battery::setStatus(QString charge) {
        if (charge != fStatus) {
            fStatus = charge;
            emit statusChanged();
            emit summaryValueChanged();
        }
    }

    const QString& Battery::getStatus() const {
        return fStatus;
    }

    int Battery::getLevel() const {
        return fLevel;
    }

    const QString& Battery::getHealth() const {
        return fHealth;
    }

    void Battery::setHealth(QString f) {
        fHealth = f;
        emit summaryValueChanged();
    }

    const QString& Battery::getTechnology() const {
        return fTechnology;
    }

    void Battery::setTechnology(QString technology) {
        fTechnology = technology;
        emit summaryValueChanged();
        emit technologyChanged();
    }
    
    int Battery::getSummaryValue() const {
        return fLevel;
    }
    

    void Battery::setLevel(int level) {
        int changed = 0;
        
        if ( fLevel != level ) {
            fLevel = level;
            emit levelChanged();
            changed++;
        }
        
        if ( changed > 0 ) {
            emit statusChanged();
            emit summaryValueChanged();
        }
    }

}
