#ifndef PROCREADER_H
#define PROCREADER_H

#include <QDir>
#include <QString>
#include <QStringList>
#include "linehandler.h"

namespace Lighthouse {

    class ProcReader
    {
    public:
        ProcReader();
        int readProcFile(QString& path, LineHandler& handler, int count, int index) const;
        QStringList getProcList();
    private:
        QStringList fProcNameFilter;
        QDir fProcDir;
    };

}

#endif // PROCREADER_H
