#include "gamecontroller.h"
#include "unitfactory.h"
#include <cmath>

GameController::GameController(UnitModel *units, MapModel *map, QObject *parent)
    : QObject(parent), m_units(units), m_map(map) {
    m_statusMessage = "Vítéte! Klikněte na start";
}
void GameController::startGame() {
    m_currentPlayer = 1;
    m_p1Gold = 0;
    m_p2Gold = 0;
    m_selectedUnitIndex = -1;
    m_isBuyingActive = false;
    m_ongoing = true;
    m_isShopOpen = false;

    switchPhase(GamePhase::BasePlacement);
}

void GameController::switchPhase(GamePhase newPhase) {
    m_phase = newPhase;
    m_selectedUnitIndex = -1;
    emit selectionChanged();

    if (m_phase == GamePhase::BasePlacement) {
        m_statusMessage = "Hráč 1: Umísti Hlavní Budovu";
    } else if (m_phase == GamePhase::UnitPurchase) {
        m_p1Gold = m_startingGold;
        m_p2Gold = m_startingGold;
        emit goldChanged();
        m_statusMessage = "Hráč 1: Nakup armádu";
        m_currentPlayer = 1;
        m_isShopOpen = true;
    } else if (m_phase == GamePhase::Combat) {
        m_isShopOpen = false;
        m_currentPlayer = 1;
        processTurnStart();
        m_statusMessage = "Boj: Hráč 1 na tahu";
    }

    emit phaseChanged();
    emit shopStateChanged();
    emit statusChanged();
    emit turnChanged();
}


void GameController::handleTileClick(int x, int y) {
    if (m_phase == GamePhase::BasePlacement) {
        handleBasePlacement(x, y);
        return;
    }

    if (m_phase == GamePhase::UnitPurchase) {
        if (!m_isBuyingActive) return;
        handleUnitPurchasePlacement(x, y);
        return;
    }

    if (m_phase == GamePhase::Combat) {
        if (m_isBuyingActive) {
            handleUnitPurchasePlacement(x, y);
        } else {
            handleCombatPhaseClick(x, y);
        }
    }
}

void GameController::setStartingGold(int gold)
{
    if(gold>=0)
    {
        m_startingGold=gold;
    }

}
void GameController::handleBasePlacement(int x, int y) {
    if (!canPlaceOnTerrain(UnitType::MainBase, x, y)) {
        m_statusMessage = "Sem nelze postavit základnu (Jen tráva)!";
        emit statusChanged();
        return;
    }
    if (m_units->findUnitIndex(x, y) != -1) return;

    m_units->addUnit(x, y, m_currentPlayer, UnitType::MainBase);

    if (m_currentPlayer == 1) {
        m_currentPlayer = 2;
        m_statusMessage = "Hráč 2: Umísti Hlavní Budovu";
    } else {
        switchPhase(GamePhase::UnitPurchase);
        return;
    }
    emit turnChanged();
    emit statusChanged();
}

void GameController::handleUnitPurchasePlacement(int x, int y) {
    int &currentGold = (m_currentPlayer == 1 ? m_p1Gold : m_p2Gold);
    int cost = UnitFactory::getUnitCost(m_selectedBuyType);

    if (currentGold < cost) {
        m_statusMessage = "Nedostatek $";
        emit statusChanged();
        return;
    }

    if (!canPlaceOnTerrain(m_selectedBuyType, x, y)) {
        m_statusMessage = "Nevhodný terén";
        emit statusChanged();
        return;
    }

    if (m_units->findUnitIndex(x, y) != -1) return;

    m_units->addUnit(x, y, m_currentPlayer, m_selectedBuyType);
    currentGold -= cost;
    emit goldChanged();

    if (m_phase == GamePhase::UnitPurchase) {
        m_statusMessage = QString("Hráč %1: Pokládá %2").arg(m_currentPlayer).arg(UnitModel::getUnitName(m_selectedBuyType));
    } else {
        m_isBuyingActive = false;
        m_isShopOpen = false;
        endTurn();
    }
    emit statusChanged();
}

void GameController::processTurnStart() {
    int income = 0;
    for (int i = 0; i < m_units->rowCount(); ++i) {
        Unit* u = m_units->getUnit(i);
        if (u->m_ownerId == m_currentPlayer) {
            if (u->m_type == UnitType::MainBase) income += 100;
            if (u->m_type == UnitType::GoldMine) income += 50;
        }
    }

    if (m_currentPlayer == 1) m_p1Gold += income;
    else m_p2Gold += income;

    emit goldChanged();
}

bool GameController::canPlaceOnTerrain(UnitType type, int x, int y) {
    int tInt = m_map->data(m_map->index(y * m_map->size() + x), MapModel::TerrainRole).toInt();
    Terrain terrain = static_cast<Terrain>(tInt);

    if (terrain == Terrain::Mountain) return false;

    if (type == UnitType::Ship) {
        return terrain == Terrain::Water;
    } else {
        return terrain == Terrain::Grass;
    }
}

void GameController::handleCombatPhaseClick(int x, int y) {
    int clickedUnitIdx = m_units->findUnitIndex(x, y);

    if (m_selectedUnitIndex == -1) {
        if (clickedUnitIdx != -1) {
            Unit* u = m_units->getUnit(clickedUnitIdx);
            if (u->m_ownerId == m_currentPlayer) {
                m_selectedUnitIndex = clickedUnitIdx;
                emit selectionChanged();
                m_statusMessage = "Vybrána: " + u->m_name;
                emit statusChanged();
                updateHighlights();
            }
        }
    }
    else {
        Unit* selected = m_units->getUnit(m_selectedUnitIndex);

        if (clickedUnitIdx != -1) {
            Unit* target = m_units->getUnit(clickedUnitIdx);
            if (target->m_ownerId == m_currentPlayer) {
                m_selectedUnitIndex = clickedUnitIdx;
                emit selectionChanged();
            } else {
                processCombat(m_selectedUnitIndex, clickedUnitIdx);
            }
        }
        else {
            if (selected->m_isBuilding) {
                m_statusMessage = "Budovy se nemohou hýbat!";
                emit statusChanged();
                return;
            }

            int dist = std::abs(selected->m_ux - x) + std::abs(selected->m_uy - y);
            if (dist <= selected->m_moveRange && canPlaceOnTerrain(selected->m_type, x, y)) {
                m_units->updatePosition(m_selectedUnitIndex, x, y);
                m_selectedUnitIndex = -1;
                emit selectionChanged();
                m_map->clearHighlights();
                endTurn();
            } else {
                m_statusMessage = "Moc daleko nebo špatný terén!";
                emit statusChanged();
            }
        }
    }
}



//!nemenit!! nearly blackbox!!
void GameController::processCombat(int attackerIdx, int targetIdx) {
    Unit* attacker = m_units->getUnit(attackerIdx);
    Unit* target = m_units->getUnit(targetIdx);

    int dist = std::abs(attacker->m_ux - target->m_ux) + std::abs(attacker->m_uy - target->m_uy);

    if (dist <= attacker->m_attackRange) {
        int damage = attacker->m_attackPower;

        m_units->updateHealth(targetIdx, target->m_health - damage);
        m_statusMessage = QString("%1 udělil %2 dmg").arg(attacker->m_name).arg(damage);

        if (target->m_health <= 0) {
            checkWinCondition(targetIdx);
            m_units->removeUnit(targetIdx);
        }

        m_selectedUnitIndex = -1;
        emit selectionChanged();
        m_map->clearHighlights();
        endTurn();
    } else {
        m_statusMessage = "Cíl je mimo dostřel!";
        emit statusChanged();
    }
}

void GameController::moveUnit(int unitIdx, int x, int y) {
    Unit* u = m_units->getUnit(unitIdx);
    int dist = std::abs(u->m_ux - x) + std::abs(u->m_uy - y);

    if (dist <= u->m_moveRange) {
        m_units->updatePosition(unitIdx, x, y);

        m_selectedUnitIndex = -1;
        endTurn();
    } else {
        m_statusMessage = "neni dosah";
        emit statusChanged();
    }
}



void GameController::checkWinCondition(int damagedUnitIdx) {
    Unit* target = m_units->getUnit(damagedUnitIdx);
    if (target && target->m_type == UnitType::MainBase && target->m_health <= 0) {
        emit gameOver(QString("Hráč %1 VYHRAL! (základnou)").arg(m_currentPlayer));
    }
}


void GameController::endTurn() {
    if (m_phase == GamePhase::Combat) {
        m_currentPlayer = (m_currentPlayer == 1 ? 2 : 1);
        processTurnStart();
        m_statusMessage = QString("Na tahu: Hráč %1").arg(m_currentPlayer);
    }

    m_selectedUnitIndex = -1;
    m_map->clearHighlights();
    m_isBuyingActive = false;
    m_isShopOpen = false;

    emit turnChanged();
    emit selectionChanged();
    emit statusChanged();
    emit shopStateChanged();
}

void GameController::resetGame() {
    m_units->clear();
    m_map->clearHighlights();
    m_currentPlayer = 1;
    m_p1Gold = 0;
    m_p2Gold = 0;
    m_selectedUnitIndex = -1;
    m_isBuyingActive = false;
    m_isShopOpen = false;
    m_phase = GamePhase::BasePlacement;
    m_statusMessage = "Vítejte! Klikněte START HRY v menu.";
    m_ongoing=false;
    emit turnChanged();
    emit statusChanged();
    emit goldChanged();
    emit shopStateChanged();
    emit phaseChanged();
    emit selectionChanged();
}

void GameController::updateHighlights() {
    m_map->clearHighlights();

    if (m_selectedUnitIndex == -1) return;

    Unit* u = m_units->getUnit(m_selectedUnitIndex);

    if (!u) return;

    int mapSize = m_map->size();
    for (int i = 0; i < mapSize * mapSize; ++i) {
        int tx = i % mapSize;
        int ty = i / mapSize;

        int dist = std::abs(u->m_ux - tx) + std::abs(u->m_uy - ty);

        if (dist == 0) continue;

        bool canMove = false;
        if (dist <= u->m_moveRange && canPlaceOnTerrain(u->m_type, tx, ty)) {
            canMove = true;
        }

        bool canAttack = false;
        bool debug=false;
        if (dist <=u->m_attackRange) {
            canAttack = true;
        }
        if
        (
            tx==u->m_ux && ty==u->m_uy
        )
        {
            m_map->setTileHighlight(i,4);
        }

        if (canMove && canAttack) {
            m_map->setTileHighlight(i, 3);
        } else if (canMove) {
            m_map->setTileHighlight(i, 1);
        } else if (canAttack) {
            m_map->setTileHighlight(i, 2);
        }
    }
}

/*Unit* attacker = m_units->getUnit(attackerIdx);
Unit* target = m_units->getUnit(targetIdx);

int dist = std::max(std::abs(attacker->m_ux - target->m_ux), std::abs(attacker->m_uy - target->m_uy));

if (dist <= attacker->m_attackRange) {*/
void GameController::selectUnitToBuy(int typeInt) {
    m_selectedBuyType = static_cast<UnitType>(typeInt);
    int cost = UnitFactory::getUnitCost(m_selectedBuyType);
    int currentGold = (m_currentPlayer == 1 ? m_p1Gold : m_p2Gold);

    if (currentGold >= cost) {
        m_isBuyingActive = true;
        m_statusMessage = "Klikni na mapu pro položení: " + UnitModel::getUnitName(m_selectedBuyType);
    } else {
        m_statusMessage = "Na toto nemáš peníze!";
        m_isBuyingActive = false;
    }
    emit statusChanged();
}

void GameController::cancelBuy() {
    m_isBuyingActive = false;
    if (m_phase == GamePhase::UnitPurchase) {
        if (m_currentPlayer == 1) {
            m_currentPlayer = 2;
            m_statusMessage = "Hráč 2: Nakup armádu";
        } else {
            switchPhase(GamePhase::Combat);
        }
        emit turnChanged();
    }
}
