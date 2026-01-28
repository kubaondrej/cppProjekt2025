#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <QAbstractListModel>
#include <QObject>


enum class UnitType {
    Soldier,
    Sniper,
    Tank,
    Ship,
    MainBase,
    GoldMine
};


class Unit : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(int health MEMBER m_health NOTIFY healthChanged)
    Q_PROPERTY(int maxHealth MEMBER m_maxHealth CONSTANT)
    Q_PROPERTY(int ownerId MEMBER m_ownerId CONSTANT)
    Q_PROPERTY(int moveRange MEMBER m_moveRange CONSTANT)
    Q_PROPERTY(int attackPower MEMBER m_attackPower CONSTANT)
    Q_PROPERTY(int attackRange MEMBER m_attackRange CONSTANT)
    Q_PROPERTY(int ux MEMBER m_ux NOTIFY positionChanged)
    Q_PROPERTY(int uy MEMBER m_uy NOTIFY positionChanged)
    Q_PROPERTY(int type MEMBER m_typeInt CONSTANT)


public:
    explicit Unit(UnitType type, int ownerId, int ux, int uy, QObject *parent = nullptr);

signals:
    void healthChanged();
    void positionChanged();

public:
    UnitType m_type;
    int m_typeInt;
    QString m_name;
    int m_ownerId;
    int m_health;
    int m_maxHealth;
    int m_moveRange;
    int m_attackPower;
    int m_attackRange;
    int m_ux;
    int m_uy;
    bool m_isBuilding;
};


class UnitModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        HealthRole,
        MaxHealthRole,
        OwnerIdRole,
        MoveRangeRole,
        AttackPowerRole,
        AttackRangeRole,
        XRole,
        YRole,
        TypeRole
    };

    explicit UnitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addUnit(int x, int y, int ownerId, UnitType type);

    bool hasUnits(int ownerId) const;
    Unit* getUnit(int index) const;
    void removeUnit(int index);
    int findUnitIndex(int x, int y) const;

    Q_INVOKABLE void updatePosition(int index, int x, int y);
    Q_INVOKABLE void updateHealth(int index, int health);

    static int getUnitCost(UnitType type);
    static QString getUnitName(UnitType type);
    void clear();
private:
    QList<Unit*> m_units;
};

#endif // UNITMODEL_H
