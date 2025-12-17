#ifndef UNITMODEL_H
#define UNITMODEL_H

#pragma once

#include <QAbstractListModel>
#include <vector>

enum class UnitType : int {
    Soldier = 0
};

struct Unit {
    int x = 0;
    int y = 0;
    UnitType type = UnitType::Soldier;
};

class UnitModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        XRole = Qt::UserRole + 1,
        YRole,
        TypeRole
    };
    Q_ENUM(Roles)

    explicit UnitModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addUnit(int x, int y);

private:
    std::vector<Unit> m_units;
};

#endif // UNITMODEL_H
