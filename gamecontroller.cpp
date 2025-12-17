#include "gamecontroller.h"
#include <cmath>

GameController::GameController(UnitModel *units, MapModel *map, QObject *parent)
    : QObject(parent), m_units(units), m_map(map)
{
    m_statusMessage = "Fáze rozmísťování: Hráč 1";
}

void GameController::handleTileClick(int x, int y) {
    if (m_isPlacementPhase) {
        if (m_units->findUnitIndex(x, y) != -1) {
            m_statusMessage = "Políčko obsazeno!";
            emit statusChanged();
            return;
        }

        m_units->addUnit(x, y, m_currentPlayer);

        if (m_currentPlayer == 1) m_p1PlacedCount++;
        else m_p2PlacedCount++;

        if (m_p1PlacedCount >= MAX_UNITS && m_p2PlacedCount >= MAX_UNITS) {
            m_isPlacementPhase = false;
            m_currentPlayer = 1;
            m_statusMessage = "BOJ ZAČÍNÁ! Na tahu Hráč 1";
            emit phaseChanged();
            emit turnChanged();
            emit statusChanged();
            return;
        }

        endTurn();
        return;
    }

    int clickedUnitIdx = m_units->findUnitIndex(x, y);

    if (m_selectedUnitIndex == -1) {
        if (clickedUnitIdx != -1) {
            Unit* u = m_units->getUnit(clickedUnitIdx);
            if (u->m_ownerId == m_currentPlayer) {
                m_selectedUnitIndex = clickedUnitIdx;
                m_statusMessage = "Vybrána jednotka. Klikni pro pohyb nebo útok.";
                emit statusChanged();
            }
        }
        return;
    }

    if (clickedUnitIdx != -1) {
        Unit* clickedUnit = m_units->getUnit(clickedUnitIdx);
        if (clickedUnit->m_ownerId == m_currentPlayer) {
            m_selectedUnitIndex = clickedUnitIdx;
        } else {
            attackUnit(m_selectedUnitIndex, clickedUnitIdx);
        }
    } else {
        moveUnit(m_selectedUnitIndex, x, y);
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
        m_statusMessage = "Moc daleko!";
        emit statusChanged();
    }
}

void GameController::attackUnit(int attackerIdx, int targetIdx) {
    Unit* attacker = m_units->getUnit(attackerIdx);
    Unit* target = m_units->getUnit(targetIdx);
    int dist = std::abs(attacker->m_ux - target->m_ux) + std::abs(attacker->m_uy - target->m_uy);

    if (dist <= 1) {
        int newHealth = target->m_health - attacker->m_attackPower;
        m_units->updateHealth(targetIdx, newHealth);
        m_statusMessage = "Útok úspěšný!";

        if (target->m_health <= 0) {
            m_units->removeUnit(targetIdx);
            m_statusMessage = "Jednotka zničena!";
            checkWinCondition();
        }

        m_selectedUnitIndex = -1;
        emit statusChanged();

        if (m_units->hasUnits(1) && m_units->hasUnits(2)) {
            endTurn();
        }
    } else {
        m_statusMessage = "Cíl je mimo dosah!";
        emit statusChanged();
    }
}

void GameController::checkWinCondition() {
    if (!m_units->hasUnits(1)) {
        emit gameOver("Hráč 2 VÍTĚZÍ!");
    } else if (!m_units->hasUnits(2)) {
        emit gameOver("Hráč 1 VÍTĚZÍ!");
    }
}

void GameController::endTurn() {
    m_currentPlayer = (m_currentPlayer == 1 ? 2 : 1);

    if (m_isPlacementPhase) {
        m_statusMessage = QString("Rozmísťování: Hráč %1 (zbývá %2)")
                              .arg(m_currentPlayer)
                              .arg(MAX_UNITS - (m_currentPlayer == 1 ? m_p1PlacedCount : m_p2PlacedCount));
    } else {
        m_statusMessage = QString("Na tahu: Hráč %1").arg(m_currentPlayer);
    }

    emit turnChanged();
    emit statusChanged();
}

void GameController::restartGame() {
    m_p1PlacedCount = 0;
    m_p2PlacedCount = 0;
    m_isPlacementPhase = true;
    m_currentPlayer = 1;
    m_statusMessage = "Nová hra: Rozmísťování Hráč 1";
    emit statusChanged();
    emit phaseChanged();
}
