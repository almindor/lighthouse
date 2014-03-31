#include "procreader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Lighthouse {

    ProcReader::ProcReader() : fProcNameFilter(), fProcDir("/proc")
    {
        fProcNameFilter << "[0-9]*";
        fProcDir.setFilter(QDir::Dirs);
        fProcDir.setNameFilters(fProcNameFilter);
    }

    int ProcReader::readProcFile(QString& path, LineHandler& handler, int count, int index) const {
        QFile file(path);
        if ( !file.open(QIODevice::ReadOnly) ) {
            qCritical() << "Unable to open proc file " << path << "\n";
            return -1;
        }

        QTextStream stream(&file);
        QString line = stream.readLine(4096);
        int c = 0;
        int i;
        while ( !line.isNull() && c < count ) {
            i = c;
            if ( index >= 0 ) {
                i = index;
            }

            int n = handler.onLine(line, i);
            if ( n != 0 ) {
                return n;
            }
            line = stream.readLine(4096);
            c++;
        }

        return 0;
    }

    QStringList ProcReader::getProcList() {
        return fProcDir.entryList();
    }

}
