#ifndef TERRAIN_TILE
#define TERRAIN_TILE

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"
#include "BulletSubsystem/BulletSubsystem.h"
#include "Ruins.h"

namespace noise
{
	namespace module
	{
		class Perlin;
		class Billow;
		class RidgedMulti;
	}
}

class TerrainTile {
public:

  // region of interest
  struct ROI {
    float x, z;
    float radius;
  };
  
  const static double SIZE_X;
  const static double SIZE_Z;
  const static int32_t VERTS_X = 90;
  const static int32_t VERTS_Z = 90;

  friend class TerrainManager; // ick, but it's a 48hr game so HA
  
  TerrainTile(iVec2 pos, noise::module::Perlin* p, Signal* pmove);
  ~TerrainTile();

  iVec2 getPosition() { return m_position; }
  Vector3 getPosition3() { return m_mesh->getPosition(); }

  void deactivate();
  void reactivate();
  void makeMesh();

  bool isActive() {
    return m_active;
  }

  inline float sampleNoise(int32_t x, int32_t z) {
    if (x >= VERTS_X + 2) { x = VERTS_X + 1; }
    if (x < -1) { x = -1; }
    if (z >= VERTS_Z + 2) { z = VERTS_Z + 1; }
    if (z < -1) { z = -1; }
    return heights[(x + 1) + (z + 1) * (VERTS_X + 3)];
  }

private:

  iVec2 m_position;
  Mesh* m_mesh;
  PhysicsObject* m_phys;
  bool m_active;

  float smoothROI(ROI r);

  float heights[(VERTS_X + 3) * (VERTS_Z + 3)];

  std::vector<Ruins*> m_ruins;

  OgreSubsystem* mGfx;
  BulletSubsystem* mPhysics;
	noise::module::Perlin* m_perlin;

  Signal* playerMv;
  
};

#endif
