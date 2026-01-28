#ifndef MAPMODEL_H
#define MAPMODEL_H
#include <QAbstractListModel>
#include <vector>

enum class Terrain : int {
    Grass = 0,
    Water = 1,
    Mountain = 2
};

struct Tile {
    Terrain terrain = Terrain::Grass;
    int highlight = 0;
};

class MapModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    enum Roles {
        TerrainRole = Qt::UserRole + 1,
        HighlightRole
    };
    explicit MapModel(QObject* parent = nullptr);
    int size() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void generate(int n);
    //zvyraznenii
    void setTileHighlight(int index, int type);
    void clearHighlights();

signals:
    void sizeChanged();

private:
    int m_size = 0;
    std::vector<Tile> m_tiles;
};

#endif // MAPMODEL_H
