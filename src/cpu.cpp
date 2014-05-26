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

#include "cpu.h"
#include <QtDebug>

namespace Lighthouse {

    CPU::CPU(QObject *parent) : QAbstractListModel(parent), fUsage() {
        fTotalUsage = 0;
        fTemperature = 0;
    }

    QHash<int, QByteArray> CPU::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[CPUUsageRole] = "cpuUsage";
        return roles;
    }

    Qt::ItemFlags CPU::flags(const QModelIndex & index) const {
        return Qt::ItemIsEnabled;
    }

    QVariant CPU::data(const QModelIndex & index, int role) const {
        const int row = index.row();
        if ( row >= 0 && (row + 1) < fUsage.size() ) {
            switch ( role ) {
                case CPUUsageRole: return fUsage.at(row + 1);
            }
        }

        return "Data[" + QString::number(index.row()) + "," + QString::number(index.column()) + "]: " + QString::number(role);
    }

    QVariant CPU::headerData(int section, Qt::Orientation orientation, int role) const {
        return "Description";
    }

    int CPU::rowCount(const QModelIndex & parent) const {
        return fUsage.size() > 0 ? fUsage.size() - 1 : 0;
    }

    int CPU::getSummaryValue() const {
       return fTotalUsage;
    }

    int CPU::getTemperature() const {
        return fTemperature;
    }

    void CPU::setUsage(const IntList& usage) {
        int usize = usage.size();
        if ( usize > 0 && fTotalUsage != usage.at(0) ) {
            fTotalUsage = usage.at(0);
            emit summaryValueChanged();
        }

        fUsage = usage;
        emit dataChanged(createIndex(0, 0), createIndex(usize - 1, 0));
    }

    void CPU::setTemperature(int degrees) {
        if ( fTemperature != degrees ) {
            fTemperature = degrees;
            emit temperatureChanged();
        }
    }

}
