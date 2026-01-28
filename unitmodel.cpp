#include "unitmodel.h"
#include "unitfactory.h"

UnitModel::UnitModel(QObject *parent) : QAbstractListModel(parent) {}


Unit::Unit(UnitType type, int ownerId, int ux, int uy, QObject *parent)
    : QObject(parent), m_type(type), m_typeInt(static_cast<int>(type)), m_ownerId(ownerId), m_ux(ux), m_uy(uy)
{

}


int UnitModel::getUnitCost(UnitType type) {
    return UnitFactory::getUnitCost(type);
}

QString UnitModel::getUnitName(UnitType type) {
    return UnitFactory::getUnitName(type);
}

//oprava predesle funkce, NEMENIT!!!
int UnitModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_units.size();
}

QVariant UnitModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_units.size()) return QVariant();

    Unit *u = m_units[index.row()];

    switch (role) {
    case NameRole: return u->m_name;
    case HealthRole: return u->m_health;
    case MaxHealthRole: return u->m_maxHealth;
    case OwnerIdRole: return u->m_ownerId;
    case MoveRangeRole: return u->m_moveRange;
    case AttackPowerRole: return u->m_attackPower;
    case AttackRangeRole: return u->m_attackRange;
    case XRole: return u->m_ux;
    case YRole: return u->m_uy;
    case TypeRole: return u->m_typeInt;
    }

    return QVariant();

}

QHash<int, QByteArray> UnitModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[HealthRole] = "health";
    roles[MaxHealthRole] = "maxHealth";
    roles[OwnerIdRole] = "ownerId";
    roles[MoveRangeRole] = "moveRange";
    roles[AttackPowerRole] = "attackPower";
    roles[AttackRangeRole] = "attackRange";
    roles[XRole] = "ux";
    roles[YRole] = "uy";
    roles[TypeRole] = "type";
    return roles;
}

void UnitModel::addUnit(int x, int y, int ownerId, UnitType type) {
    beginInsertRows(QModelIndex(), m_units.size(), m_units.size());
    Unit* u = UnitFactory::createUnit(type, ownerId, x, y, this);
    m_units.append(u);
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
    m_units[index]->m_ux = x;
    m_units[index]->m_uy = y;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {XRole, YRole});
}

void UnitModel::updateHealth(int index, int health) {
    if (index < 0 || index >= m_units.size()) return;
    m_units[index]->m_health = health;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {HealthRole});
}

void UnitModel::clear() {
    beginResetModel();
    qDeleteAll(m_units);
    m_units.clear();
    endResetModel();
}
