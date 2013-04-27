#ifndef PlayState_H
#define PlayState_H

#include "Common.h"
#include "OryxGameState.h"
#include "FPSCamera.h"
#include "GUISubsystem/GUIPanel.h"
#include "GUISubsystem/GUICaption.h"
#include "GUISubsystem/GUIButton.h"

class PlayState : public GameState
{
public:

	PlayState();
	virtual ~PlayState();

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

	// Plain 'ol FPS camera
	FPSCamera* mCam;
	
	// GUI Mesh
	ScreenMesh* mUI;

	// Debug GUI captions
	Caption* mFpsText;

};

#endif
