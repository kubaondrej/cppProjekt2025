#include "MapModel.h"

MapModel::MapModel(QObject* parent)
    : QAbstractListModel(parent) {}

int MapModel::size() const {
    return m_size;
}

int MapModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return static_cast<int>(m_tiles.size());
}

QVariant MapModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return {};

    const auto& tile = m_tiles[static_cast<size_t>(index.row())];

    if (role == TerrainRole)
        return static_cast<int>(tile.terrain);

    return {};
}

QHash<int, QByteArray> MapModel::roleNames() const {
    return { { TerrainRole, "terrain" } };
}

void MapModel::generate(int n) {
    if (n <= 0) return;

    beginResetModel();

    m_size = n;
    m_tiles.assign(static_cast<size_t>(n * n), Tile{});

    // demo generování: voda po okrajích, zbytek tráva
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            if (x == 0 || y == 0 || x == n - 1 || y == n - 1) {
                m_tiles[static_cast<size_t>(y * n + x)].terrain = Terrain::Water;
            }
        }
    }

    endResetModel();
    emit sizeChanged();
}
