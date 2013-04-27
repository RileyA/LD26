#include "TerrainManager.h"
#include <libnoise/noise.h>

TerrainManager::TerrainManager() {
  createSlot("playerMoved",this,&TerrainManager::playerMoved);
	m_perlin = new noise::module::Perlin();
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
  createTerrainTile(iVec2(0,0));
}

TerrainManager::~TerrainManager() {
  delete m_perlin;
}

void TerrainManager::update(Real delta) {
  // ... 
}

void TerrainManager::playerMoved(const Message& msg) {
  if(const MessageAny<Vector3>* ms = message_cast<Vector3>(msg))
  {
    Vector3 p = ms->data;
    Vector3 offset(TerrainTile::SIZE_X / -2.0, 0.f, TerrainTile::SIZE_Z / -2.0);
    Vector3 pos = p;// - offset;
    iVec2 ipos(static_cast<int>(floor(pos.x)) / static_cast<int>(TerrainTile::SIZE_X / 2),
      static_cast<int>(floor(pos.z)) / static_cast<int>(TerrainTile::SIZE_Z / 2));
    //std::cout<<"pos: "<<ipos.x<<" "<<ipos.y<<"\n";
    updateTiles(ipos);
  }
}

TerrainTile* TerrainManager::createTerrainTile(iVec2 pos) {
  
    MeshData d;

    // center pos
    Vector3 center_pos(pos.x * TerrainTile::SIZE_X, 0.f, pos.y * TerrainTile::SIZE_Z);

    // upper corner
    Vector3 corner_pos(pos.x * TerrainTile::SIZE_X - TerrainTile::SIZE_X / 2.0, 0.f, 
      pos.y * TerrainTile::SIZE_Z - TerrainTile::SIZE_Z / 2.0);

    Vector3 offset(TerrainTile::SIZE_X / -2.0, 0.f, TerrainTile::SIZE_Z / -2.0);

    double step_x = TerrainTile::SIZE_X / TerrainTile::VERTS_X;
    double step_z = TerrainTile::SIZE_Z / TerrainTile::VERTS_Z;

    // create grid of vertices..
    for (int z = 0; z <= TerrainTile::VERTS_Z; ++z) {
      for (int x = 0; x <= TerrainTile::VERTS_X; ++x) {
        // TODO sample perlin for height
        Vector3 lpos = Vector3(x * step_x, -2, z * step_z);
  
        Vector3 abs_pos = corner_pos + lpos;
        double height = m_perlin->GetValue(
          static_cast<double>(abs_pos.x) * 0.005,
          0.0,
          static_cast<double>(abs_pos.z) * 0.005);
        lpos.y = height * 20;

        d.vertex(lpos + offset);
        d.diffuse.push_back(0.2f + ((double)x / TerrainTile::VERTS_X) * 0.7f);
        d.diffuse.push_back(0.5f);
        d.diffuse.push_back(0.1f + ((double)z / TerrainTile::VERTS_Z) * 0.7f);
        d.diffuse.push_back(1.f);
      }
    }

    // add indices
    for (int z = 0; z <= TerrainTile::VERTS_Z - 1; ++z) {
      for (int x = 0; x <= TerrainTile::VERTS_X - 1; ++x) {
        uint32_t v1 = x + z * (TerrainTile::VERTS_X + 1);
        uint32_t v2 = (x + 1) + z * (TerrainTile::VERTS_X + 1);
        uint32_t v3 = x + (z + 1) * (TerrainTile::VERTS_X + 1);
        uint32_t v4 = (x + 1) + (z + 1) * (TerrainTile::VERTS_X + 1);

        // TODO: verify winding
        d.indices.push_back(v1);
        d.indices.push_back(v3);
        d.indices.push_back(v2);
        d.indices.push_back(v2);
        d.indices.push_back(v3);
        d.indices.push_back(v4);
      }
    }

    Mesh* m = mGfx->createMesh(d);
    mGfx->getRootSceneNode()->addChild(m);
    m->setMaterialName("Terrain");
    m_tiles[pos] = new TerrainTile(m, pos);
    return m_tiles[pos];
}

void TerrainManager::killTerrainTile(iVec2 pos) {
  if (m_tiles.count(pos))
    killTerrainTile(m_tiles[pos]);
}

void TerrainManager::killTerrainTile(TerrainTile* t) {
  // C++: where your friends can touch your private members...
  mGfx->getRootSceneNode()->removeChild(t->m_mesh);
  mGfx->destroySceneNode(t->m_mesh);
  delete t;

  //if (m_tiles.count(t->getPosition())) {
  //  m_tiles.erase(m_tiles.find(t->getPosition()));
  //}
}

void TerrainManager::updateTiles(iVec2 pos) {
  for (std::map<iVec2, TerrainTile*>::iterator it = m_tiles.begin();
    it != m_tiles.end();) {
    iVec2 p = it->second->getPosition();
    if (abs(p.x - pos.x) > UNPAGE_RADIUS) {
      killTerrainTile(it->second);
      m_tiles.erase(it++);
    } else {
      ++it;
    }
  }

  for (int i = -PAGE_RADIUS; i <= PAGE_RADIUS; ++i)
    for (int j = -PAGE_RADIUS; j <= PAGE_RADIUS; ++j) {
      iVec2 p = pos + iVec2(i, j);
      if (!m_tiles.count(p)) {
        createTerrainTile(p);
      }
  }
}

