#include "Common.h"
#include "PlayState.h"
#include "FPSCamera.h"

PlayState::PlayState()
	:mGfx(0), mAudio(0), mGUI(0), mInput(0), mPhysics(0)
{}
//---------------------------------------------------------------------------

PlayState::~PlayState()
{
}
//---------------------------------------------------------------------------

void PlayState::init()
{
	srand(time(0));

	// grab pointers to e'rythang
	mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
	mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
	mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
	mGUI = dynamic_cast<GUISubsystem*>(mEngine->getSubsystem("GUISubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(mEngine->getSubsystem("BulletSubsystem"));

	// start up input, grab the mouse
	mInput->initInput(mGfx->getWindowHandle(), true);

	// start up bullet for collision detection
	mPhysics->startSimulation();

	// background and fog colors
	Colour col = Colour(10/255.f,15/255.f,20/255.f);
	Colour col2 = Colour(10/255.f,15/255.f,20/255.f);
	mGfx->setBackgroundColor(col2);
	//mGfx->setLinearFog(150.f,200.f,col2);

	// standard FPS-style camera (no character controller just yet)
	mCam = new FPSCamera();
  mCam->mPosNode->setPosition(Vector3(0,20,3));

	// set up debug overlay
	Batch* b = mGUI->createBatch("test", "../media/gui/gui.oyster");

  //Mesh* m = mGfx->createMesh("Cube.mesh");
  //mGfx->getRootSceneNode()->addChild(m);
	
	Caption* c = new Caption(b, 0);
	c->setCaption("FPS: 60");
	c->setPosition(Vector2(0.01f, 0.94f));
	mFpsText = c;

	// hook it up with the rendering system
	mUI = mGfx->createScreenMesh("DebugUI");
	b->getSignal("update")->addListener(mUI->getSlot("update"));
	mUI->setHidden(false);

  m_terrain = new TerrainManager();

  mCam->getSignal("moved")->addListener(m_terrain->getSlot("playerMoved"));
}
//---------------------------------------------------------------------------

void PlayState::update(Real delta)
{
	// fps cam update
	Vector3 moveVect = mCam->mCamera->getAbsoluteDirection()*7*delta*
		(mInput->isKeyDown("KC_W")-mInput->isKeyDown("KC_S"))
		+mCam->mCamera->getAbsoluteRight()*7*delta*
		(mInput->isKeyDown("KC_D")-mInput->isKeyDown("KC_A"));

	Real len = moveVect.normalize();
	len *= 10.f;

  mCam->setPosition(mCam->getPosition() + moveVect * len);

	// same o' same o'
	if(mInput->wasKeyPressed("KC_ESCAPE"))
		mEngine->shutdown();

	// screenshots
	//if(mInput->wasKeyPressed("KC_P"))
	//	mGfx->takeScreenshot(TimeManager::getPtr()->getTimestamp());

	// update debug overlay
	//mFpsText->setCaption("FPS: "+ StringUtils::toString(1.f/delta));
}
//---------------------------------------------------------------------------

void PlayState::deinit() {
}
//---------------------------------------------------------------------------

