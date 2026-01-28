#include "unitmodel.h"

UnitModel::UnitModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

Unit::Unit(UnitType type, int ownerId, int ux, int uy, QObject *parent)
    : QObject(parent), m_type(type), m_typeInt(static_cast<int>(type)), m_ownerId(ownerId), m_ux(ux), m_uy(uy)
{
    switch (type) {
    case UnitType::Soldier:
        m_name = "Voják";
        m_maxHealth = 40; m_attackPower = 15; m_moveRange = 3; m_attackRange = 2;
        m_isBuilding = false;
        break;
    case UnitType::Sniper:
        m_name = "Sniper";
        m_maxHealth = 20; m_attackPower = 40; m_moveRange = 2; m_attackRange = 5;
        m_isBuilding = false;
        break;
    case UnitType::Tank:
        m_name = "Tank";
        m_maxHealth = 100; m_attackPower = 50; m_moveRange = 3; m_attackRange = 2;
        m_isBuilding = false;
        break;
    case UnitType::Ship:
        m_name = "Loď";
        m_maxHealth = 80; m_attackPower = 35; m_moveRange = 4; m_attackRange = 3;
        m_isBuilding = false;
        break;
    case UnitType::MainBase:
        m_name = "Základna";
        m_maxHealth = 200; m_attackPower = 0; m_moveRange = 0; m_attackRange = 0;
        m_isBuilding = true;
        break;
    case UnitType::GoldMine:
        m_name = "Důl";
        m_maxHealth = 100; m_attackPower = 0; m_moveRange = 0; m_attackRange = 0;
        m_isBuilding = true;
        break;
    }
    m_health = m_maxHealth;
}


int UnitModel::getUnitCost(UnitType type) {
    switch (type) {
    case UnitType::Soldier: return 100;
    case UnitType::Sniper: return 200;
    case UnitType::Tank: return 400;
    case UnitType::Ship: return 350;
    case UnitType::GoldMine: return 300;
    case UnitType::MainBase: return 0;
    default: return 9999;
    }
}

QString UnitModel::getUnitName(UnitType type) {
    switch (type) {
    case UnitType::Soldier: return "Voják (100g)";
    case UnitType::Sniper: return "Sniper (200g)";
    case UnitType::Tank: return "Tank (400g)";
    case UnitType::Ship: return "Loď (350g)";
    case UnitType::GoldMine: return "Důl (300g)";
    default: return "";
    }
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
    case TypeRole:        return u->m_typeInt;
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
    roles[TypeRole] = "type";
    return roles;
}

void UnitModel::addUnit(int x, int y, int ownerId, UnitType type) {
    beginInsertRows(QModelIndex(), m_units.size(), m_units.size());
    m_units.append(new Unit(type, ownerId, x, y, this));
    endInsertRows();
}
bool UnitModel::hasUnits(int ownerId) const {
    for (const auto* unit : m_units) if (unit->m_ownerId == ownerId) return true;
    return false;
}

Unit* UnitModel::getUnit(int index) const {
    return (index < 0 || index >= m_units.size()) ? nullptr : m_units[index];
}

int UnitModel::findUnitIndex(int x, int y) const {
    for (int i = 0; i < m_units.size(); ++i) if (m_units[i]->m_ux == x && m_units[i]->m_uy == y) return i;
    return -1;
}

void UnitModel::removeUnit(int index) {
    if (index < 0 || index >= m_units.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    delete m_units.takeAt(index);
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
