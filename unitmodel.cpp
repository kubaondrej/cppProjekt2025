#include "UnitModel.h"

UnitModel::UnitModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int UnitModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_units.size();
}

QVariant UnitModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_units.size())
        return QVariant();

    const Unit &u = m_units[index.row()];

    switch (role) {
    case NameRole:        return u.name;
    case HealthRole:      return u.health;
    case OwnerIdRole:     return u.ownerId;
    case MoveRangeRole:   return u.moveRange;
    case AttackPowerRole: return u.attackPower;
    case XRole:           return u.ux;
    case YRole:           return u.uy;
    }

    return QVariant();
}

QHash<int, QByteArray> UnitModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole]        = "name";
    roles[HealthRole]      = "health";
    roles[OwnerIdRole]     = "ownerId";
    roles[MoveRangeRole]   = "moveRange";
    roles[AttackPowerRole] = "attackPower";
    roles[XRole]           = "ux";
    roles[YRole]           = "uy";
    return roles;
}

void UnitModel::addUnit(int x, int y)
{
    beginInsertRows(QModelIndex(), m_units.size(), m_units.size());
    m_units.append({
        "Unit",   // name
        100,      // health
        1,        // ownerId
        3,        // moveRange
        10,       // attackPower
        x, y      // position
    });
    endInsertRows();
}
