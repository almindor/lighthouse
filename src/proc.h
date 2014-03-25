#ifndef PROC_H
#define PROC_H

#include <QThread>
#include <QVariantList>
#include <QVector>

namespace Lighthouse {

    class Proc : public QThread
    {
        public:
            Proc();
            ~Proc();
            Q_OBJECT
            QVariantList getCPUUsage();
        private:

            bool fQuit;
            QVariantList fCPUUsage;
            QVector<unsigned long long> fCPUTicks;
            int fInterval;
            int fCPUCount;


            void run() Q_DECL_OVERRIDE;
            void procCPUActivity();
            unsigned long long parseCPUParts(QStringList& parts, int flags);
            int getProcessorCount();
        signals:
            void CPUUsageChanged(QVariantList usage);
    };

}

#endif // PROC_H
