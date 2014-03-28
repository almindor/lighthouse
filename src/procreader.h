#ifndef PROCREADER_H
#define PROCREADER_H

#include <QString>
#include "linehandler.h"

namespace Lighthouse {

    class ProcReader
    {
    public:
        ProcReader();
        int readProcFile(QString& path, LineHandler& handler, int count);
    };

}

#endif // PROCREADER_H
