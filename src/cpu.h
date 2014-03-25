#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QVariantList>

namespace Lighthouse {

    class CPU : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QVariantList usage READ getUsage NOTIFY usageChanged)
        Q_PROPERTY(const QString coverInfo READ getCoverInfo NOTIFY coverInfoChanged)
        Q_PROPERTY(bool pageActive READ getPageActive WRITE setPageActive)
    public:
        QVariantList getUsage();
        QString getCoverInfo();
        void setPageActive(bool active);
        bool getPageActive();
    private:
        QVariantList fUsage;
        int fUsageLevel;
        bool fPageActive;
    public slots:
        void setUsage(QVariantList usage);
    signals:
        void usageChanged(QVariantList usage);
        void coverInfoChanged(QString info);
    };

}

#endif // CPU_H
