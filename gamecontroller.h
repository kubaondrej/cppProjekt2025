#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include "unitmodel.h"
#include "mapmodel.h"

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
    void gameOver(QString winnerMessage);

private:
    UnitModel *m_units;
    MapModel *m_map;

    int m_currentPlayer = 1;
    int m_selectedUnitIndex = -1;
    QString m_statusMessage;

    bool m_isPlacementPhase = true;
    int m_p1PlacedCount = 0;
    int m_p2PlacedCount = 0;
    const int MAX_UNITS = 5;

    void moveUnit(int unitIdx, int x, int y);
    void attackUnit(int attackerIdx, int targetIdx);
    void endTurn();
    void checkWinCondition();
};

#endif // GAMECONTROLLER_H
