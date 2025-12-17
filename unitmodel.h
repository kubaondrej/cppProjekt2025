#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <QAbstractListModel>
#include <QObject>

class Unit : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(int health MEMBER m_health NOTIFY healthChanged)
    Q_PROPERTY(int ownerId MEMBER m_ownerId CONSTANT)
    Q_PROPERTY(int moveRange MEMBER m_moveRange CONSTANT)
    Q_PROPERTY(int attackPower MEMBER m_attackPower CONSTANT)

    Q_PROPERTY(int ux MEMBER m_ux NOTIFY positionChanged)
    Q_PROPERTY(int uy MEMBER m_uy NOTIFY positionChanged)

public:
    explicit Unit(QString name,
                  int ownerId,
                  int moveRange,
                  int attackPower,
                  int ux,
                  int uy,
                  QObject *parent = nullptr)
        : QObject(parent),
        m_name(name),
        m_ownerId(ownerId),
        m_health(100),
        m_moveRange(moveRange),
        m_attackPower(attackPower),
        m_ux(ux),
        m_uy(uy)
    {}

signals:
    void healthChanged();
    void positionChanged();

public:
    QString m_name;
    int m_ownerId;
    int m_health;
    int m_moveRange;
    int m_attackPower;
    int m_ux;
    int m_uy;
};


class UnitModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        HealthRole,
        OwnerIdRole,
        MoveRangeRole,
        AttackPowerRole,
        XRole,
        YRole
    };
    Unit* getUnit(int index) const;
    void removeUnit(int index);
    int findUnitIndex(int x, int y) const;

    explicit UnitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addUnit(int x, int y);

private:
    QList<Unit*> m_units;
};

#endif // UNITMODEL_H
