#include "TerrainManager.h"
#include <libnoise/noise.h>

TerrainManager::TerrainManager(Signal* pmove) {
  playerMv = pmove;
  pmove->addListener(createSlot("playerMoved",this,&TerrainManager::playerMoved));
	m_perlin = new noise::module::Perlin();
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
  createTerrainTile(iVec2(0,0));
}

TerrainManager::~TerrainManager() {
  delete m_perlin;
}

void TerrainManager::update(Real delta) {
}

void TerrainManager::playerMoved(const Message& msg) {
  if(const MessageAny<Vector3>* ms = message_cast<Vector3>(msg))
  {
    Vector3 p = ms->data;
    Vector3 offset(TerrainTile::SIZE_X / -2.0, 0.f, TerrainTile::SIZE_Z / -2.0);
    Vector3 pos = p;// - offset;
    iVec2 ipos(static_cast<int>(floor(pos.x)) / static_cast<int>(TerrainTile::SIZE_X),
      static_cast<int>(floor(pos.z)) / static_cast<int>(TerrainTile::SIZE_Z));
    updateTiles(ipos);
  }
}

TerrainTile* TerrainManager::createTerrainTile(iVec2 pos) {
  TerrainTile* t = new TerrainTile(pos, m_perlin, playerMv);
  m_tiles[pos] = t;
  return t;
}

void TerrainManager::killTerrainTile(iVec2 pos) {
  // derp
}

void TerrainManager::killTerrainTile(TerrainTile* t) {
  // derp
}

void TerrainManager::updateTiles(iVec2 pos) {
  for (std::map<iVec2, TerrainTile*>::iterator it = m_tiles.begin();
    it != m_tiles.end();) {
    iVec2 p = it->second->getPosition();
    if (it->second->isActive() 
        && (abs(p.x - pos.x) > UNPAGE_RADIUS
            || abs(p.y - pos.y) > UNPAGE_RADIUS)) {
      it->second->deactivate();
      return;
    } else {
      ++it;
    }
  }

  for (int i = -PAGE_RADIUS; i <= PAGE_RADIUS; ++i)
    for (int j = -PAGE_RADIUS; j <= PAGE_RADIUS; ++j) {
      iVec2 p = pos + iVec2(i, j);
      if (!m_tiles.count(p)) {
        createTerrainTile(p);
        //std::cout<<"created " << i << " " << j << "\n";
        return;
      } else if (!m_tiles[p]->isActive()) {
        //std::cout<<"activated\n";
        m_tiles[p]->reactivate();
        return;
      }
  }
}

void TerrainManager::updateAllTiles(iVec2 pos) {
  for (std::map<iVec2, TerrainTile*>::iterator it = m_tiles.begin();
    it != m_tiles.end();) {
    iVec2 p = it->second->getPosition();
    if (it->second->isActive() 
        && (abs(p.x - pos.x) > UNPAGE_RADIUS
            || abs(p.y - pos.y) > UNPAGE_RADIUS)) {
      it->second->deactivate();
    } else {
      ++it;
    }
  }

  for (int i = -PAGE_RADIUS; i <= PAGE_RADIUS; ++i)
    for (int j = -PAGE_RADIUS; j <= PAGE_RADIUS; ++j) {
      iVec2 p = pos + iVec2(i, j);
      if (!m_tiles.count(p)) {
        createTerrainTile(p);
        //std::cout<<"created " << i << " " << j << "\n";
      } else if (!m_tiles[p]->isActive()) {
        //std::cout<<"activated\n";
        m_tiles[p]->reactivate();
      }
  }
}

double TerrainManager::sampleNoise(Vector3 pos) {

  // nice, slow rolling hills
  double acc = m_perlin->GetValue(
    static_cast<double>(pos.x) * 0.0003,
    0.0,
    static_cast<double>(pos.z) * 0.0003) * 5;
  // a little bit of rougher terrain
  acc += m_perlin->GetValue(
    static_cast<double>(pos.x) * 0.00001,
    0.0,
    static_cast<double>(pos.z) * 0.00001) * 
  m_perlin->GetValue(
    static_cast<double>(pos.x) * 0.01,
    0.0,
    static_cast<double>(pos.z) * 0.01) * 2.5;
  // a little bit of rougher noise
  return std::max(acc, 0.0);
}

