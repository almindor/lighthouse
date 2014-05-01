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
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Lighthouse {

    ProcReader::ProcReader() : fProcNameFilter()
    {
        fProcNameFilter << "[0-9]*";
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
            if ( n < 0 ) {
                return n;
            } else if ( n == 100 ) { // skip rest, we're done
                break;
            }
            line = stream.readLine(4096);
            c++;
        }

        return 0;
    }

    int ProcReader::getProcCount() const {
        QDir dir("/proc");
        dir.setFilter(QDir::Dirs);
        dir.setNameFilters(fProcNameFilter);

        return dir.count();
    }

    const QStringList ProcReader::getProcList() const {
        QDir dir("/proc");
        dir.setSorting(QDir::Name);
        dir.setFilter(QDir::Dirs);
        dir.setNameFilters(fProcNameFilter);

        return dir.entryList();
    }

}
