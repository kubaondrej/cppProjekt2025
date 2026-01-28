#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include "unitmodel.h"
#include "mapmodel.h"

enum class GamePhase {
    BasePlacement,
    UnitPurchase,
    Combat
};


class GameController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentPlayer READ currentPlayer NOTIFY turnChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusChanged)
    Q_PROPERTY(bool isPlacementPhase READ isPlacementPhase NOTIFY phaseChanged)

public:
    explicit GameController(UnitModel *units, MapModel *map, QObject *parent = nullptr);

    int currentPlayer() const { return m_currentPlayer; }
    QString statusMessage() const { return m_statusMessage; }
    bool isPlacementPhase() const { return m_isPlacementPhase; }

    Q_INVOKABLE void handleTileClick(int x, int y);
    Q_INVOKABLE void restartGame();

signals:
    void turnChanged();
    void statusChanged();
    void phaseChanged();
    void selectionChanged();
    void goldChanged();
    void shopStateChanged();
    void gameOver(QString winnerMessage);

private:

    //faze hry, obcas preskakuje random
    GamePhase m_phase = GamePhase::BasePlacement;
    UnitType m_selectedBuyType = UnitType::Soldier;

    UnitModel *m_units;
    MapModel *m_map;
    bool m_isShopOpen = false;

    int m_currentPlayer = 1;
    int m_selectedUnitIndex = -1;
    QString m_statusMessage;

    bool m_isPlacementPhase = true;
    int m_p1PlacedCount = 0;
    int m_p2PlacedCount = 0;
    const int MAX_UNITS = 5;
    bool m_isBuyingActive = false;

    int m_p1Gold = 0;
    int m_p2Gold = 0;
    void moveUnit(int unitIdx, int x, int y);
    void attackUnit(int attackerIdx, int targetIdx);
    void checkWinCondition();


    void processTurnStart();
    void endTurn();
    void checkWinCondition(int damagedUnitIdx);
    void processCombat(int attackerIdx, int targetIdx);

    void handleBasePlacement(int x, int y);
    void handleUnitPurchasePlacement(int x, int y);
    void handleCombatPhaseClick(int x, int y);

    bool canPlaceOnTerrain(UnitType type, int x, int y);
    void switchPhase(GamePhase newPhase);
    void updateHighlights();
};

#endif // GAMECONTROLLER_H
