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
        Q_PROPERTY(QString statusShort READ getStatusShort NOTIFY statusChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
        Q_PROPERTY(QString health READ getHealth NOTIFY healthChanged)
        Q_PROPERTY(QString technology READ getTechnology NOTIFY technologyChanged)
	public:
		explicit Battery(QObject *parent = 0);

        int getLevel() const;
        const QString& getStatus() const;
        const QString getStatusShort() const;
        int getSummaryValue() const;
        const QString& getHealth() const;
        const QString& getTechnology() const;
	private:
        QString fHealth;
        QString fTechnology;
        int fLevel;
        QString fStatus;
	public slots:
        void setHealth(QString heal);
        void setTechnology(QString tech);
        void setLevel(int level);
        void setStatus(QString charge);
	signals:
        void healthChanged();
        void levelChanged();
        void statusChanged();
        void technologyChanged();
        void summaryValueChanged();
	};
}
#endif // BATTERY_H
