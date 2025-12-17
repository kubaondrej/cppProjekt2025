#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QAbstractListModel>
#include <vector>

enum class Terrain : int {
    Grass = 0,
    Water = 1
};

struct Tile {
    Terrain terrain = Terrain::Grass;
};

class MapModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    enum Roles {
        TerrainRole = Qt::UserRole + 1
    };

    explicit MapModel(QObject* parent = nullptr);

    int size() const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void generate(int n);

signals:
    void sizeChanged();

private:
    int m_size = 0;
    std::vector<Tile> m_tiles;
};

#endif // MAPMODEL_H
