#include "Common.h"
#include "MenuState.h"
#include "PlayState.h"
#include "FPSCamera.h"
#include "Powerup.h"


MenuState::MenuState()
	:mGfx(0), mAudio(0), mGUI(0), mInput(0), mPhysics(0)
{}
//---------------------------------------------------------------------------

MenuState::~MenuState()
{
}
//---------------------------------------------------------------------------

void MenuState::init()
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
	//mPhysics->startSimulation();

	Colour sky = Colour(0.f,0.f,0.f);
	mGfx->setBackgroundColor(sky);

	// set up debug overlay
	b = mGUI->createBatch("test", "../media/gui/gui.oyster");

	Caption* c = new Caption(b, 0);
	c->setCaption("Welcome");
	c->setPosition(Vector2(0.01f, 0.9f));
	c = new Caption(b, 0);
	c->setCaption("Energy: 100");
	c->setPosition(Vector2(0.01f, 0.94f));

  Panel* mEnergyBar = new Panel(b, 1);
	mEnergyBar->setPosition(Vector2(0.f, 0.96f));
  mEnergyBar->setScale(0.32, 0.05);
  mEnergyBar->setHorizontalAlign(HA_CENTER);
  mEnergyBar->setColor(Colour(0.2,1,1, 1.f));
  mEnergyBar->setSolidColor();
  Panel* mEnergyBarCap = new Panel(b, 0);
	mEnergyBarCap->setPosition(Vector2(0.f, 0.96f));
  mEnergyBarCap->setScale(0.4, 0.05);
  mEnergyBarCap->setHorizontalAlign(HA_CENTER);
  mEnergyBarCap->setColor(Colour(0.2,0.2,0.2, 0.5f));
  mEnergyBarCap->setSolidColor();

	// hook it up with the rendering system
	mUI = mGfx->createScreenMesh("DebugUI");
	b->getSignal("update")->addListener(mUI->getSlot("update"));
	mUI->setHidden(false);

}
//---------------------------------------------------------------------------

void MenuState::update(Real delta)
{
	if(mInput->wasKeyPressed("KC_RETURN")) {
    mEngine->addState(new PlayState());
    mEngine->endCurrentState();
  }

	// same o' same o'
	if(mInput->wasKeyPressed("KC_ESCAPE"))
		mEngine->shutdown();

	// screenshots
	if(mInput->wasKeyPressed("KC_P"))
		mGfx->takeScreenshot(TimeManager::getPtr()->getTimestamp());
}
//---------------------------------------------------------------------------

void MenuState::deinit() {
  // clean up???
}
//---------------------------------------------------------------------------

