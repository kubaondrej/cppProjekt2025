#include "unitmodel.h"

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

    Unit *u = m_units[index.row()];

    switch (role) {
    case NameRole:        return u->m_name;
    case HealthRole:      return u->m_health;
    case OwnerIdRole:     return u->m_ownerId;
    case MoveRangeRole:   return u->m_moveRange;
    case AttackPowerRole: return u->m_attackPower;
    case XRole:           return u->m_ux;
    case YRole:           return u->m_uy;
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

    Unit *u = new Unit("Unit", 1, 3, 10, x, y, this);
    m_units.append(u);

    endInsertRows();
}
