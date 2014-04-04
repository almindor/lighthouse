#include "battery.h"
#include <QtDebug>

namespace Lighthouse {
    Battery::Battery(QObject *parent) :
        QObject(parent)
    {
            fLevel=0;
            fStatus="Not readed";
            fhealth="Not readed";
            ftechno="Not readed";
    }
    
    void Battery::setStatus(QString charg) {
        if(charg!=fStatus){
            fStatus=charg;
            emit summaryValueChanged(getSummaryValue());
        }else{
            return;
        }
    }

    QString Battery::getStatus() const {
        return fStatus;
    }

    int Battery::getLevel() const {
        return fLevel;
    }

    QString Battery::getHealth() const {
        return fhealth;
    }

    void Battery::setHealth(QString f) {
        fhealth = f;
        emit summaryValueChanged(getSummaryValue());
    }

    QString Battery::getTechno() const {
        return ftechno;
    }

    void Battery::setTechno(QString f) {
        ftechno = f;
        emit summaryValueChanged(getSummaryValue());
    }
    
    QString Battery::getSummaryValue() const {
        QString b = "health:"+fhealth + "; "+fStatus+":"+QString::number(fLevel)+"%";
        return b;
    }
    

    void Battery::setLevel(int level) {
        int changed = 0;
        
        if ( fLevel != level ) {
            fLevel = level;
            emit levelChanged(level);
            changed++;
        }
        
        if ( changed > 0 ) {
            emit summaryValueChanged(getSummaryValue());
        }
    }

}
