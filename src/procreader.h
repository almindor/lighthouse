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

#ifndef PROCREADER_H
#define PROCREADER_H

#include <QString>
#include <QStringList>
#include "linehandler.h"

namespace Lighthouse {

    class ProcReader
    {
    public:
        ProcReader();
        int readProcFile(QString& path, LineHandler& handler, int count, int index) const;
        int getProcCount() const;
        const QStringList getProcList() const;
    private:
        QStringList fProcNameFilter;
    };

}

#endif // PROCREADER_H
