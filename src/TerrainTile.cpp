#include "TerrainTile.h"

TerrainTile::TerrainTile(Mesh* m, iVec2 position)
  :m_mesh(m), m_position(position) {
  Vector3 pos(position.x * SIZE_X, 0.f, position.y * SIZE_Z);
  m_mesh->setPosition(pos);
}

TerrainTile::~TerrainTile() {
  // we'll let the TerrainManager handle cleanup
}

