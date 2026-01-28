#include "gamecontroller.h"
#include <cmath>

GameController::GameController(UnitModel *units, MapModel *map, QObject *parent)
    : QObject(parent), m_units(units), m_map(map)
{
    m_statusMessage = "rozmistuje hrac 1";
}

void GameController::startGame() {
    m_currentPlayer = 1;
    m_p1Gold = 0;
    m_p2Gold = 0;
    m_selectedUnitIndex = -1;
    m_isBuyingActive = false;
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
        m_p1Gold = 1000;
        m_p2Gold = 1000;
        emit goldChanged();
        m_statusMessage = "Hráč 1: Nakup armádu (1000g)";
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
    int cost = UnitModel::getUnitCost(m_selectedBuyType);

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
//TODO
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

void GameController::attackUnit(int attackerIdx, int targetIdx) {
    Unit* attacker = m_units->getUnit(attackerIdx);
    Unit* target = m_units->getUnit(targetIdx);
    int dist = std::abs(attacker->m_ux - target->m_ux) + std::abs(attacker->m_uy - target->m_uy);

    if (dist <= 2) {
        int newHealth = target->m_health - attacker->m_attackPower;
        m_units->updateHealth(targetIdx, newHealth);
        m_statusMessage = "utok uspech";

        if (target->m_health <= 0) {
            m_units->removeUnit(targetIdx);
            m_statusMessage = "bum!!! jednotka znicena";
            checkWinCondition();
        }

        m_selectedUnitIndex = -1;
        emit statusChanged();

        if (m_units->hasUnits(1) && m_units->hasUnits(2)) {
            endTurn();
        }
    } else {
        m_statusMessage = "cil je mimo dosah";
        emit statusChanged();
    }
}

void GameController::checkWinCondition() {
    if (!m_units->hasUnits(1)) {
        emit gameOver("hrac 2 vyhral");
    } else if (!m_units->hasUnits(2)) {
        emit gameOver("hrac 1 vyhral");
    }
}

void GameController::endTurn() {
    m_currentPlayer = (m_currentPlayer == 1 ? 2 : 1);

    if (m_isPlacementPhase) {
        m_statusMessage = QString("rozmistovani: hrac %1 (zbyva %2)")
        .arg(m_currentPlayer)
            .arg(MAX_UNITS - (m_currentPlayer == 1 ? m_p1PlacedCount : m_p2PlacedCount));
    } else {
        m_statusMessage = QString("Na tahu: Hráč %1").arg(m_currentPlayer);
    }

    emit turnChanged();
    emit statusChanged();
}

void GameController::resetGame() {
    m_currentPlayer = 1;
    m_p1Gold = 0;
    m_p2Gold = 0;
    m_selectedUnitIndex = -1;
    m_isBuyingActive = false;
    m_isShopOpen = false;
    m_phase = GamePhase::BasePlacement;
    m_statusMessage = "Vítejte! Klikněte START HRY v menu.";
    emit turnChanged();
    emit statusChanged();
    emit goldChanged();
    emit shopStateChanged();
    emit phaseChanged();
    emit selectionChanged();
}

void GameController::selectUnitToBuy(int typeInt) {
    m_selectedBuyType = static_cast<UnitType>(typeInt);
    int cost = UnitModel::getUnitCost(m_selectedBuyType);
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
