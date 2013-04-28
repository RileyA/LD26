
#ifndef MenuState_H
#define MenuState_H

#include "Common.h"
#include "Enemy.h"
#include "OryxGameState.h"
#include "FPSCamera.h"
#include "GUISubsystem/GUIPanel.h"
#include "GUISubsystem/GUICaption.h"
#include "GUISubsystem/GUIButton.h"
#include "TerrainManager.h"
#include "Ruins.h"

class MenuState : public GameState
{
public:

	MenuState();
	virtual ~MenuState();

	virtual void init();
	virtual void update(Real delta);
	virtual void deinit();

private:

	// Subsystems
	OgreSubsystem* mGfx;
	OISSubsystem* mInput;
	ALSubsystem* mAudio;
	GUISubsystem* mGUI;
	BulletSubsystem* mPhysics;

	// GUI Mesh
	ScreenMesh* mUI;
  Batch* b;
};

#endif
