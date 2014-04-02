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

#ifndef MEMORY_H
#define MEMORY_H

#include <QObject>

namespace Lighthouse {

    class Memory : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(unsigned long long free READ getFree NOTIFY freeChanged)
        Q_PROPERTY(unsigned long long total READ getTotal NOTIFY totalChanged)
        Q_PROPERTY(int summaryValue READ getSummaryValue NOTIFY summaryValueChanged)
    public:
        explicit Memory(QObject *parent = 0);

        unsigned long long getFree() const;
        unsigned long long getTotal() const;
        int getSummaryValue() const;
    private:
        unsigned long long fFree;
        unsigned long long fTotal;
    signals:
        void freeChanged(int free);
        void totalChanged(int total);
        void summaryValueChanged(int value);
    public slots:
        void setMemory(unsigned long long total, unsigned long long free);
    };

}

#endif // MEMORY_H
