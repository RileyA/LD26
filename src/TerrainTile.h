#ifndef TERRAIN_TILE
#define TERRAIN_TILE

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"

class TerrainTile {
public:
  
  const static double SIZE_X = 100.0;
  const static double SIZE_Z = 100.0;
  const static size_t VERTS_X = 50;
  const static size_t VERTS_Z = 50;

  friend class TerrainManager; // ick, but it's a 48hr game so HA
  
  TerrainTile(Mesh* m, iVec2 pos);
  ~TerrainTile();

  iVec2 getPosition() { return m_position; }
  Vector3 getPosition3() { return m_mesh->getPosition(); }

private:

  iVec2 m_position;
  Mesh* m_mesh;
  
};

#endif
