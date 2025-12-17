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

void UnitModel::addUnit(int x, int y, int ownerId)
{
    beginInsertRows(QModelIndex(), m_units.size(), m_units.size());
    Unit *u = new Unit("Soldier", ownerId, 3, 25, x, y, this);
    m_units.append(u);
    endInsertRows();
}

bool UnitModel::hasUnits(int ownerId) const {
    for (const auto* unit : m_units) {
        if (unit->m_ownerId == ownerId) return true;
    }
    return false;
}

Unit* UnitModel::getUnit(int index) const {
    if (index < 0 || index >= m_units.size()) return nullptr;
    return m_units[index];
}

int UnitModel::findUnitIndex(int x, int y) const {
    for (int i = 0; i < m_units.size(); ++i) {
        if (m_units[i]->m_ux == x && m_units[i]->m_uy == y) return i;
    }
    return -1;
}

void UnitModel::removeUnit(int index) {
    if (index < 0 || index >= m_units.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    Unit* u = m_units.takeAt(index);
    delete u;
    endRemoveRows();
}

void UnitModel::updatePosition(int index, int x, int y) {
    if (index < 0 || index >= m_units.size()) return;

    Unit* u = m_units[index];
    u->m_ux = x;
    u->m_uy = y;
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {XRole, YRole});
}

void UnitModel::updateHealth(int index, int health) {
    if (index < 0 || index >= m_units.size()) return;

    Unit* u = m_units[index];
    u->m_health = health;

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {HealthRole});
}
