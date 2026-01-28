#include "unitfactory.h"

#include "unitfactory.h"

Unit* UnitFactory::createUnit(UnitType type, int ownerId, int x, int y, QObject* parent)
{
    Unit* unit = new Unit(type, ownerId, x, y, parent);
    configureUnitStats(unit, type);
    return unit;
}

void UnitFactory::configureUnitStats(Unit* unit, UnitType type)
{
    switch (type) {
    case UnitType::Soldier:
        unit->m_name = "Voják";
        unit->m_maxHealth = 40;
        unit->m_attackPower = 15;
        unit->m_moveRange = 3;
        unit->m_attackRange = 2;
        unit->m_isBuilding = false;
        break;
    case UnitType::Sniper:
        unit->m_name = "Sniper";
        unit->m_maxHealth = 20;
        unit->m_attackPower = 40;
        unit->m_moveRange = 2;
        unit->m_attackRange = 5;
        unit->m_isBuilding = false;
        break;
    case UnitType::Tank:
        unit->m_name = "Tank";
        unit->m_maxHealth = 100;
        unit->m_attackPower = 50;
        unit->m_moveRange = 3;
        unit->m_attackRange = 2;
        unit->m_isBuilding = false;
        break;
    case UnitType::Ship:
        unit->m_name = "Loď";
        unit->m_maxHealth = 80;
        unit->m_attackPower = 35;
        unit->m_moveRange = 4;
        unit->m_attackRange = 3;
        unit->m_isBuilding = false;
        break;
    case UnitType::MainBase:
        unit->m_name = "Základna";
        unit->m_maxHealth = 200;
        unit->m_attackPower = 0;
        unit->m_moveRange = 0;
        unit->m_attackRange = 0;
        unit->m_isBuilding = true;
        break;
    case UnitType::GoldMine:
        unit->m_name = "Důl";
        unit->m_maxHealth = 100;
        unit->m_attackPower = 0;
        unit->m_moveRange = 0;
        unit->m_attackRange = 0;
        unit->m_isBuilding = true;
        break;
    }
    unit->m_health = unit->m_maxHealth;
}

int UnitFactory::getUnitCost(UnitType type) {
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

QString UnitFactory::getUnitName(UnitType type) {
    int cost = getUnitCost(type);
    QString suffix = (cost > 0) ? QString(" (%1g)").arg(cost) : "";
    switch (type) {
    case UnitType::Soldier: return "Voják" + suffix;
    case UnitType::Sniper: return "Sniper" + suffix;
    case UnitType::Tank: return "Tank" + suffix;
    case UnitType::Ship: return "Loď" + suffix;
    case UnitType::GoldMine: return "Důl" + suffix;
    default: return "";
    }
}
