#ifndef UNITFACTORY_H
#define UNITFACTORY_H
#include "unitmodel.h"

class UnitFactory
{
public:
    static Unit* createUnit(UnitType type, int ownerId, int x, int y, QObject* parent = nullptr);
    static int getUnitCost(UnitType type);
    static QString getUnitName(UnitType type);

private:
    static void configureUnitStats(Unit* unit, UnitType type);
};

#endif // UNITFACTORY_H
