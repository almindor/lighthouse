#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
//#include "types.h"

namespace Lighthouse {
	class Battery : public QObject
	{
		Q_OBJECT
        Q_PROPERTY(int level READ getLevel NOTIFY levelChanged)
        Q_PROPERTY(QString status READ getStatus NOTIFY statusChanged)
        Q_PROPERTY(QString summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
        Q_PROPERTY(QString health READ getHealth NOTIFY healthChanged)
        Q_PROPERTY(QString techno READ getTechno)
	public:
		explicit Battery(QObject *parent = 0);

        int getLevel() const;
        QString getStatus() const;
        QString getSummaryValue() const;
        QString getHealth() const;
        QString getTechno() const;
	private:
        QString fhealth;
        QString ftechno;
        int fLevel;
        QString fStatus;
	public slots:
        void setHealth(QString heal);
        void setTechno(QString tech);
        void setLevel(int level);
        void setStatus(QString charge);
	signals:
        void healthChanged(QString value);
        void levelChanged(int level);
        void statusChanged();
        void summaryValueChanged(QString value);
	};
}
#endif // BATTERY_H
