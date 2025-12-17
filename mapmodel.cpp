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
            int r = QRandomGenerator::global()->bounded(100);
            tile.terrain = (r < 25) ? Terrain::Water : Terrain::Grass;
        }
    }

    int continents = std::max(1, n / 4);

    for (int i = 0; i < continents; ++i) {
        int cx = QRandomGenerator::global()->bounded(2, n - 2);
        int cy = QRandomGenerator::global()->bounded(2, n - 2);
        int radius = QRandomGenerator::global()->bounded(2, 5);

        for (int y = cy - radius; y <= cy + radius; ++y) {
            for (int x = cx - radius; x <= cx + radius; ++x) {
                if (x < 0 || y < 0 || x >= n || y >= n)
                    continue;

                int dx = x - cx;
                int dy = y - cy;

                if (dx * dx + dy * dy <= radius * radius) {
                    m_tiles[static_cast<size_t>(y * n + x)].terrain = Terrain::Grass;
                }
            }
        }
    }

    endResetModel();
    emit sizeChanged();
}
