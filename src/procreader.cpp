#include "procreader.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

namespace Lighthouse {

    ProcReader::ProcReader()
    {
    }

    int ProcReader::readProcFile(QString& path, LineHandler& handler, int count) {
        QFile file(path);
        if ( !file.open(QIODevice::ReadOnly) ) {
            qCritical() << "Unable to open proc file " << path << "\n";
            return -1;
        }

        QTextStream stream(&file);
        QString line = stream.readLine(4096);
        int c = 0;
        while ( !line.isNull() && c < count ) {
            line = stream.readLine(4096);
            int n = handler.onLine(line, c);
            if ( n != 0 ) {
                return n;
            }
            c++;
        }

        return 0;
    }

}
