#include "MapModel.h"
#include <QRandomGenerator>

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

    if (role == TerrainRole) {
        return static_cast<int>(tile.terrain);
    }

    return {};
}

QHash<int, QByteArray> MapModel::roleNames() const {
    return {
        { TerrainRole, "terrain" }
    };
}

void MapModel::generate(int n) {
    if (n <= 0) return;

    beginResetModel();

    m_size = n;
    m_tiles.assign(static_cast<size_t>(n * n), Tile{});

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            auto& tile = m_tiles[static_cast<size_t>(y * n + x)];

            if (x == 0 || x == n - 1 || y == 0 || y == n - 1) {
                tile.terrain = Terrain::Water;
            }
            else {
                bool isCenter = (x > n/4 && x < 3*n/4 && y > n/4 && y < 3*n/4);
                int r = QRandomGenerator::global()->bounded(100);

                if (isCenter && r < 30) {
                    tile.terrain = Terrain::Mountain;
                } else if (r < 10) {
                    tile.terrain = Terrain::Water;
                } else {
                    tile.terrain = Terrain::Grass;
                }
            }
        }
    }

    endResetModel();
    emit sizeChanged();
}

void MapModel::setTileHighlight(int index, int type) {
    if (index < 0 || index >= static_cast<int>(m_tiles.size())) return;

    if (m_tiles[index].highlight != type) {
        m_tiles[index].highlight = type;
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), {HighlightRole});
    }
}

void MapModel::clearHighlights() {
    for (size_t i = 0; i < m_tiles.size(); ++i) {
        if (m_tiles[i].highlight != 0) {
            m_tiles[i].highlight = 0;
        }
    }
    emit dataChanged(createIndex(0, 0), createIndex(m_tiles.size()-1, 0), {HighlightRole});
}


