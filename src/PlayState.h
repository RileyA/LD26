#ifndef PlayState_H
#define PlayState_H

#include "Common.h"
#include "Enemy.h"
#include "OryxGameState.h"
#include "FPSCamera.h"
#include "GUISubsystem/GUIPanel.h"
#include "GUISubsystem/GUICaption.h"
#include "GUISubsystem/GUIButton.h"
#include "TerrainManager.h"
#include "Ruins.h"

class PlayState : public GameState
{
public:

	PlayState();
	virtual ~PlayState();

	virtual void init();
	virtual void update(Real delta);
	virtual void deinit();

  // icky global, but hey it's a 48hr game...
  static float playerEnergy;
  static float playerEnergyCap;

private:

	// Subsystems
	OgreSubsystem* mGfx;
	OISSubsystem* mInput;
	ALSubsystem* mAudio;
	GUISubsystem* mGUI;
	BulletSubsystem* mPhysics;

	// Plain 'ol FPS camera
	FPSCamera* mCam;
	
	// GUI Mesh
	ScreenMesh* mUI;

	// Debug GUI captions
	Caption* mFpsText;
	Caption* mEnergyText;
  Panel* mEnergyBar;
  Panel* mEnergyBarCap;

  TerrainManager* m_terrain;

  PhysicsObject* character;
  QuantaController* cct;

  Batch* b;

  Enemy* enemy;

  Real timeLived;
  bool gameover;

  float walkTime;
  
  MeshData footsteps;
  uint32_t foot_idx;
  bool left_foot;
  float footTime;
  Mesh* fsmesh;
  std::vector<Enemy*> enemies;

};

#endif
