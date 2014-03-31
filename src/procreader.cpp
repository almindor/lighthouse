/*
    Copyright (C) 2014 Ales Katona.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
