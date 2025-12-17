#include "UnitModel.h"

UnitModel::UnitModel(QObject* parent)
    : QAbstractListModel(parent) {}

int UnitModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(m_units.size());
}

QVariant UnitModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return {};

    const Unit& u = m_units[index.row()];

    switch (role) {
    case XRole: return u.x;
    case YRole: return u.y;
    case TypeRole: return static_cast<int>(u.type);
    }

    return {};
}

QHash<int, QByteArray> UnitModel::roleNames() const {
    return {
        { XRole, "ux" },
        { YRole, "uy" },
        { TypeRole, "utype" }
    };
}

void UnitModel::addUnit(int x, int y) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_units.push_back(Unit{x, y});
    endInsertRows();
}
