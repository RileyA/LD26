#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"
#include "TerrainTile.h"

namespace noise
{
	namespace module
	{
		class Perlin;
		class Billow;
		class RidgedMulti;
	}
}

class TerrainManager : public Object {
public:

  const static int PAGE_RADIUS = 4;
  const static int UNPAGE_RADIUS = 5;
  
  TerrainManager(Signal* pmove);
  ~TerrainManager();

  void update(Real delta);
  void playerMoved(const Message& m);

private:

  TerrainTile* createTerrainTile(iVec2 pos);
  void killTerrainTile(iVec2 pos);
  void killTerrainTile(TerrainTile* t);
  void updateTiles(iVec2 pos);
  void updateAllTiles(iVec2 pos);
  double sampleNoise(Vector3 pos);

	noise::module::Perlin* m_perlin;
	//noise::module::RidgedMulti* m_ridged;
	//noise::module::Billow* m_billow;

  std::map<iVec2, TerrainTile*> m_tiles;

  OgreSubsystem* mGfx;
  BulletSubsystem* mPhysics;

  Signal* playerMv;

  
};

#endif
