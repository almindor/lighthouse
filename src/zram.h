/*
    Copyright (C) 2018 Lukáš Karas <lukas.karas@centrum.cz>

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

#ifndef ZRAM_H
#define ZRAM_H

#include <QObject>

namespace Lighthouse {

    class Zram : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(qint64 diskSize READ getDiskSize NOTIFY zramChanged)
        Q_PROPERTY(qint64 data READ getData NOTIFY zramChanged)
        Q_PROPERTY(qint64 compressed READ getCompressed NOTIFY zramChanged)
    public:
        explicit Zram(QObject *parent = 0);

        qint64 getDiskSize() const;
        qint64 getData() const;
        qint64 getCompressed() const;

    public slots:
        void onZramChanged(qint64 disksize, qint64 data, qint64 compr);

    signals:
        void zramChanged();

    private:
        qint64 disksize{0};
        qint64 data{0};
        qint64 compr{0};
    };
}

#endif // ZRAM_H
