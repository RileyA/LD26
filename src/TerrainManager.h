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

  const static int PAGE_RADIUS = 2;
  const static int UNPAGE_RADIUS = 3;
  
  TerrainManager();
  ~TerrainManager();

  void update(Real delta);
  void playerMoved(const Message& m);

private:

  TerrainTile* createTerrainTile(iVec2 pos);
  void killTerrainTile(iVec2 pos);
  void killTerrainTile(TerrainTile* t);
  void updateTiles(iVec2 pos);

	noise::module::Perlin* m_perlin;
	//noise::module::RidgedMulti* m_ridged;
	//noise::module::Billow* m_billow;

  std::map<iVec2, TerrainTile*> m_tiles;

  OgreSubsystem* mGfx;
  
};

#endif
