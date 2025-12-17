#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <QAbstractListModel>

class UnitModel : public QAbstractListModel
{
    Q_OBJECT

public:
    struct Unit {
        QString name;
        int health;
        int ownerId;
        int moveRange;
        int attackPower;
        int ux;
        int uy;
    };

    enum Roles {
        NameRole = Qt::UserRole + 1,
        HealthRole,
        OwnerIdRole,
        MoveRangeRole,
        AttackPowerRole,
        XRole,
        YRole
    };

    explicit UnitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addUnit(int x, int y);

private:
    QList<Unit> m_units;
};

#endif // UNITMODEL_H
