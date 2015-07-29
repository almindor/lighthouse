#include "battery.h"
#include <QtDebug>

namespace Lighthouse {
    Battery::Battery(QObject *parent) :
        QObject(parent)
    {
            fLevel = 0;
            fStatus = tr("Unavailable", "battery status");
            fHealth = tr("Unavailable", "battery status");
            fTechnology = tr("Unavailable", "battery status");
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

    const QString Battery::getStatusShort() const {
        const unsigned short ss = fStatus.length() > 0 ? fStatus.at(0).unicode() : 'u';

        switch (ss) {
            case 'C':
            case 'c': return "↑";
            case 'D':
            case 'd': return "↓";
        }

        return "~";
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
