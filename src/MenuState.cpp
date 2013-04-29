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
	c->setCaption("Depletion");
	c->setPosition(Vector2(0.f, 0.2f));
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
  c->setColor(Colour(1.f, 0.7f,0.85f));


	c = new Caption(b, 0);
	c->setCaption("WASD: Move, SHIFT: sprint, MOUSE: look, ESC: quit");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.4f));

	c = new Caption(b, 0);
	c->setCaption("Press Enter to Play");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.5f));

	c = new Caption(b, 0);
	c->setCaption("v This is your energy bar, it depletes over time.");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.7f));

	c = new Caption(b, 0);
	c->setCaption("It runs out faster when sprinting or being hurt.");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.74f));

	c = new Caption(b, 0);
	c->setCaption("The game is over if it reaches zero.");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.78f));

	c = new Caption(b, 0);
	c->setCaption("You can replenish it with energy packs,");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.82f));

	c = new Caption(b, 0);
	c->setCaption("which are found in structures around the world.");
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);
	c->setPosition(Vector2(0.f, 0.86f));

	c = new Caption(b, 0);
	c->setCaption("Survive as long as you can.");
	c->setPosition(Vector2(0.f, 0.9f));
  c->setHorizontalAlign(HA_CENTER);
  c->setTextHorizontalAlign(HA_CENTER);


  Panel* mEnergyBar = new Panel(b, 1);
	mEnergyBar->setPosition(Vector2(0.f, 0.96f));
  mEnergyBar->setScale(0.32, 0.02);
  mEnergyBar->setHorizontalAlign(HA_CENTER);
  mEnergyBar->setColor(Colour(0.2,1,1, 1.f));
  mEnergyBar->setSolidColor();
  Panel* mEnergyBarCap = new Panel(b, 0);
	mEnergyBarCap->setPosition(Vector2(0.f, 0.96f));
  mEnergyBarCap->setScale(0.4, 0.02);
  mEnergyBarCap->setHorizontalAlign(HA_CENTER);
  mEnergyBarCap->setColor(Colour(0.2,0.2,0.2, 0.5f));
  mEnergyBarCap->setSolidColor();

	// hook it up with the rendering system
	mUI = mGfx->createScreenMesh("DebugUI");
	b->getSignal("update")->addListener(mUI->getSlot("update"));
	mUI->setHidden(false);
  delay = false;

}
//---------------------------------------------------------------------------

void MenuState::update(Real delta)
{
  if (delay) {
    mEngine->addState(new PlayState());
    mEngine->endCurrentState();
  }

	if(mInput->wasKeyPressed("KC_RETURN")) {
    Caption* c = new Caption(b, 0);
    c->setCaption("LOADING...");
    c->setHorizontalAlign(HA_CENTER);
    c->setTextHorizontalAlign(HA_CENTER);
    c->setPosition(Vector2(0.f, 0.57f));
    c->setColor(Colour(0.9, 0.8,0.4));
    delay = true;
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

