#include "Common.h"
#include "PlayState.h"
#include "FPSCamera.h"
#include "Powerup.h"

#define NUM_FOOTSTEPS 1000


float PlayState::playerEnergy = 100.f;
float PlayState::playerEnergyCap = 100.f;

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
  gameover = false;

	// grab pointers to e'rythang
	mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
	mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
	mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
	mGUI = dynamic_cast<GUISubsystem*>(mEngine->getSubsystem("GUISubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(mEngine->getSubsystem("BulletSubsystem"));

  mAudio->loadSound("../media/audio/explode.ogg");
  mAudio->loadSound("../media/audio/attack.ogg");
  mAudio->loadSound("../media/audio/jump.ogg");
  mAudio->loadSound("../media/audio/fail.ogg");
  mAudio->loadSound("../media/audio/powerup.ogg");
  mAudio->loadSound("../media/audio/powerup2.ogg");

	// start up input, grab the mouse
	mInput->initInput(mGfx->getWindowHandle(), true);

	// start up bullet for collision detection
	mPhysics->startSimulation();

  playerEnergy = 100.f;

	// background and fog colors
	//Colour col = Colour(10/255.f,15/255.f,20/255.f);
	Colour col2 = Colour(10/255.f,15/255.f,20/255.f);
	Colour sky = Colour(0.f,0.f,0.f);
	mGfx->setBackgroundColor(sky);
	//mGfx->setLinearFog(3.f,5.f,sky);

	// standard FPS-style camera (no character controller just yet)
	mCam = new FPSCamera();
  mCam->mPosNode->setPosition(Vector3(0,30,0));

	// set up debug overlay
	b = mGUI->createBatch("test", "../media/gui/gui.oyster");

	/*Caption* c = new Caption(b, 0);
	c->setCaption("FPS: 60");
	c->setPosition(Vector2(0.01f, 0.9f));
	mFpsText = c;
	c = new Caption(b, 0);
	c->setCaption("Energy: 100");
	c->setPosition(Vector2(0.01f, 0.94f));
	mEnergyText = c;*/

  mEnergyBar = new Panel(b, 1);
	mEnergyBar->setPosition(Vector2(0.f, 0.96f));
  mEnergyBar->setScale(0.4, 0.05);
  mEnergyBar->setHorizontalAlign(HA_CENTER);
  mEnergyBar->setColor(Colour(0.2,1,1, 1.f));
  mEnergyBar->setSolidColor();
  mEnergyBarCap = new Panel(b, 0);
	mEnergyBarCap->setPosition(Vector2(0.f, 0.96f));
  mEnergyBarCap->setScale(0.4, 0.05);
  mEnergyBarCap->setHorizontalAlign(HA_CENTER);
  mEnergyBarCap->setColor(Colour(0.2,0.2,0.2, 0.5f));
  mEnergyBarCap->setSolidColor();

	// hook it up with the rendering system
	mUI = mGfx->createScreenMesh("DebugUI");
	b->getSignal("update")->addListener(mUI->getSlot("update"));
	mUI->setHidden(false);

  //character = mPhysics->createSphere(0.5f, Vector3(0,10,0));
  //mCam->getSignal("moved")->addListener(m_terrain->getSlot("playerMoved"));
  cct = mPhysics->createQuantaCCT(Vector3(0,20,0));
  walkTime = 0.f;

  mGfx->addCompositor("Depth");
  mGfx->setCompositor("Depth", true);
  timeLived = 0.f;

  // im a bad person
  if (Ruins::RUINS.empty()) {
    Ruins::RUINS.push_back(new Ruin());
    Ruins::RUINS.back()->mesh = "Ruins004.mesh";
    Ruins::RUINS.back()->phys_mesh = "../media/models/Ruins004.obj";
    Ruins::RUINS.back()->radius = 14;
    Ruins::RUINS.back()->d.loadObj(Ruins::RUINS.back()->phys_mesh);
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(0,0.015,0), Vector3(3.5, 0, 5)));
    Ruins::RUINS.push_back(new Ruin());
    Ruins::RUINS.back()->mesh = "Ruins005.mesh";
    Ruins::RUINS.back()->phys_mesh = "../media/models/Ruins005.obj";
    Ruins::RUINS.back()->radius = 22;
    Ruins::RUINS.back()->d.loadObj(Ruins::RUINS.back()->phys_mesh);
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(0,0,-2), Vector3(3.5, 0, 5)));
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(0,3.9,4), Vector3(4, 0, 3.25)));
    Ruins::RUINS.push_back(new Ruin());
    Ruins::RUINS.back()->mesh = "Ruins006.mesh";
    Ruins::RUINS.back()->phys_mesh = "../media/models/Ruins006.obj";
    Ruins::RUINS.back()->radius = 14;
    Ruins::RUINS.back()->d.loadObj(Ruins::RUINS.back()->phys_mesh);
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(0,0.188,-0.088), Vector3(0.75, 0, 2.75)));
    Ruins::RUINS.push_back(new Ruin());
    Ruins::RUINS.back()->mesh = "Ruins007.mesh";
    Ruins::RUINS.back()->phys_mesh = "../media/models/Ruins007.obj";
    Ruins::RUINS.back()->radius = 6;
    Ruins::RUINS.back()->d.loadObj(Ruins::RUINS.back()->phys_mesh);
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(0,0.127,0), Vector3(0.75, 0, 0.5)));
    Ruins::RUINS.push_back(new Ruin());
    Ruins::RUINS.back()->mesh = "Ruins008.mesh";
    Ruins::RUINS.back()->phys_mesh = "../media/models/Ruins008.obj";
    Ruins::RUINS.back()->radius = 17;
    Ruins::RUINS.back()->d.loadObj(Ruins::RUINS.back()->phys_mesh);
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(3.879,0.127,0), Vector3(1.75, 0, 2.25)));
    Ruins::RUINS.back()->spawns.push_back(std::make_pair(Vector3(-3.161,0.127,0), Vector3(1.75, 0, 2.25)));
  }
  m_terrain = new TerrainManager(mCam->getSignal("moved"));

  for (int i = 0; i < 100; ++i) {
    m_terrain->updateAllTiles(iVec2(0,0));
  }

  footsteps.addTexcoordSet();
  for (int i = 0; i < NUM_FOOTSTEPS; ++i) {
    footsteps.vertex(Vector3(0,-1000,0));
    footsteps.vertex(Vector3(0,-1000,0));
    footsteps.vertex(Vector3(0,-1000,0));
    footsteps.vertex(Vector3(0,-1000,0));
    footsteps.texcoords[0].push_back(0.f);
    footsteps.texcoords[0].push_back(0.f);
    footsteps.texcoords[0].push_back(1.f);
    footsteps.texcoords[0].push_back(0.f);
    footsteps.texcoords[0].push_back(0.f);
    footsteps.texcoords[0].push_back(1.f);
    footsteps.texcoords[0].push_back(1.f);
    footsteps.texcoords[0].push_back(1.f);
    footsteps.indices.push_back(i * 4 + 0);
    footsteps.indices.push_back(i * 4 + 1);
    footsteps.indices.push_back(i * 4 + 2);
    footsteps.indices.push_back(i * 4 + 1);
    footsteps.indices.push_back(i * 4 + 3);
    footsteps.indices.push_back(i * 4 + 2);
  }
  footsteps.bbox[0] = -10000000;
  footsteps.bbox[1] = -10000000;
  footsteps.bbox[2] = -10000000;
  footsteps.bbox[3] = 10000000;
  footsteps.bbox[4] = 10000000;
  footsteps.bbox[5] = 10000000;
  fsmesh = mGfx->createMesh(footsteps);
  mGfx->getRootSceneNode()->addChild(fsmesh);
  left_foot = true;
  foot_idx = 0;
  footTime = 0.3f;

  TimeManager::getPtr()->setTimeSpeed(1.f);
}
//---------------------------------------------------------------------------

void PlayState::update(Real delta)
{
  timeLived += delta;

	// fps cam update
	Vector3 moveVect = mCam->mCamera->getAbsoluteDirection()*7*delta*
		(mInput->isKeyDown("KC_W")-mInput->isKeyDown("KC_S"))
		+mCam->mCamera->getAbsoluteRight()*7*delta*
		(mInput->isKeyDown("KC_D")-mInput->isKeyDown("KC_A"));

  if (!moveVect.isZeroLength()) {
    walkTime += delta;
    footTime -= delta;
  }

  moveVect.y = 0;
	Real len = moveVect.normalize();
	len *= 2.f;

  if ((timeLived > 30.f && enemies.size() < 1)
      || (timeLived > 120.f && enemies.size() < 2)
      || (timeLived > 240.f && enemies.size() < 3)) {
    Vector3 ppos = mCam->getPosition();
    Vector3 pdir = mCam->getDirection();
    pdir.y = 0;
    ppos.y = 0;
    pdir *= -1; // spawn behind player
    pdir.normalize();
    pdir *= 250.f;
    enemies.push_back(new Enemy(ppos + pdir));
    mCam->getSignal("moved")->addListener(enemies.back()->getSlot("playerMoved"));
  }

  bool sprint = mInput->isKeyDown("KC_LSHIFT");

  cct->move(moveVect, sprint ? 20.f : 7.f);

  /*if (sprint) {
    mCam->mCamera->setFOV(50.f);
  } else {
    mCam->mCamera->setFOV(75.f);
  }*/

  //mCam->setPosition(mCam->getPosition() + moveVect * len);
  //mCam->setPosition(character->getPosition());

  float sp = sprint ? 20.f / 5.f : 1.f;
  Vector3 headBob(0.f, 0.8f + sin(walkTime * 12.f) * sp * 0.1f, 0.f);

  if (!moveVect.isZeroLength()) {
    playerEnergy -= delta * (sprint ? 1.5f : 0.3f);
  } else {
    playerEnergy -= delta * 0.1f;
  }

  if (footTime < 0.f) {
    footTime = 0.2f;
    left_foot = !left_foot;

    RaycastReport r = mPhysics->raycast(cct->getPosition() + 
      (left_foot ? mCam->mCamera->getAbsoluteLeft() * 0.5f : mCam->mCamera->getAbsoluteRight() * 0.5f ), Vector3::NEGATIVE_UNIT_Y, 2.f, 0x1, 0x1);

    if (r.hit && r.group == 0x2) {
      ++foot_idx;
      if (foot_idx >= NUM_FOOTSTEPS) {
        foot_idx = 0;
      }

      Vector3 start = Vector3::UNIT_Y;
      Vector3 norm = r.normal;
      norm.normalize();
      //Quaternion q = start.getRotationTo(norm);

      float yoff = 0.02f;

      r.position += Vector3(0,0,0);
      float* buf = &footsteps.vertices[foot_idx * 12];
      /*if (left_foot) {
        dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/leftfoot.ogg")->setGain(0.f, 1.f, 1.f);;
      } else {
        dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/rightfoot.ogg")->setGain(0.f, 1.f, 1.f);;
      }*/

      Vector3 zb = moveVect;
      moveVect.normalize();
      Vector3 yb = r.normal;
      Vector3 xb = yb.crossProduct(zb);

      Quaternion q;
      q.fromAxes(xb,yb,zb);
      Real sz = 0.25f;

      Vector3 vp = r.position + q * Vector3(-sz,yoff,sz);
      footsteps.updateBBox(vp);
      *buf++ = vp.x; *buf++ = vp.y; *buf++ = vp.z;

      vp = r.position + q * Vector3(sz,yoff,sz);
      footsteps.updateBBox(vp);
      *buf++ = vp.x; *buf++ = vp.y; *buf++ = vp.z;

      vp = r.position + q * Vector3(-sz,yoff,-sz);
      footsteps.updateBBox(vp);
      *buf++ = vp.x; *buf++ = vp.y; *buf++ = vp.z;

      vp = r.position + q * Vector3(sz,yoff,-sz);
      footsteps.updateBBox(vp);
      *buf++ = vp.x; *buf++ = vp.y; *buf++ = vp.z;

      fsmesh->updatePositions(footsteps);
      fsmesh->setMaterialName("footsteps");
    }

  }

  mCam->setPosition(cct->getPosition() + headBob);

	if(mInput->wasKeyPressed("KC_RETURN") && gameover) {
    mEngine->addState(new PlayState());
    mEngine->endCurrentState();
  }

	// same o' same o'
	if(mInput->wasKeyPressed("KC_ESCAPE"))
		mEngine->shutdown();

	if(mInput->wasKeyPressed("KC_SPACE")) {
    cct->jump(12.f);
    dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/jump.ogg")->setGain(0.f, 1.f, 1.f);
  }
	// screenshots
	if(mInput->wasKeyPressed("KC_P"))
		mGfx->takeScreenshot(TimeManager::getPtr()->getTimestamp());

	// update debug overlay
	//mFpsText->setCaption("FPS: "+ StringUtils::toString(1.f/delta));
  if (playerEnergy <= 0.f && !gameover) {
    gameover = true;
    Panel* pan = new Panel(b, 0);
    pan->setPosition(Vector2(0.f, 0.3f));
    pan->setScale(0.6, 0.4);
    pan->setHorizontalAlign(HA_CENTER);
    pan->setColor(Colour(0.8,0.8,0.8,0.8f));
    pan->setSolidColor();
    Caption* c = new Caption(b, 0);
    c->setCaption("GAME OVER!");
    c->setColor(Colour(0,0,0,1));
    c->setPosition(Vector2(0.f, 0.35f));
    c->setScale(0.5, 0.1);
    c->setHorizontalAlign(HA_CENTER);
    c->setTextHorizontalAlign(HA_CENTER);

    std::ifstream prevBest("highscore.txt");
    float prevTime = 0.f;
    bool hasPrev = false;
    if (prevBest.good()) {
      prevBest >> prevTime;
      hasPrev = true;
    }

    Real pmins = floor(prevTime / 60.f);
    Real psecs = floor(prevTime) - (pmins * 60.f);

    Real mins = floor(timeLived / 60.f);
    Real secs = floor(timeLived) - (mins * 60.f);
    c = new Caption(b, 0);
    c->setCaption("You lasted " + StringUtils::toString(mins) + " mins "
      + StringUtils::toString(secs) + " seconds");
    c->setColor(Colour(0,0,0,1));
    c->setPosition(Vector2(0.f, 0.45f));
    c->setScale(0.5, 0.1);
    c->setHorizontalAlign(HA_CENTER);
    c->setTextHorizontalAlign(HA_CENTER);

    if (hasPrev) {
      c = new Caption(b, 0);
      c->setColor(Colour(0,0,0,1));
      c->setPosition(Vector2(0.f, 0.51f));
      c->setScale(0.5, 0.1);
      c->setHorizontalAlign(HA_CENTER);
      c->setTextHorizontalAlign(HA_CENTER);
      if (mins > pmins || (mins == pmins && secs > psecs)) {
        c->setCaption("That's a new best time!");
        std::ofstream newBest("highscore.txt");
        if (newBest.good()) {
          newBest << timeLived;
        }
      } else {
        c->setCaption("Previous best: " + StringUtils::toString(pmins) + " mins "
          + StringUtils::toString(psecs) + " seconds");
      }
    } else {
      std::ofstream newBest("highscore.txt");
      if (newBest.good()) {
        newBest << timeLived;
      }
    }

    c = new Caption(b, 0);
    c->setCaption("ENTER - Play Again");
    c->setColor(Colour(0,0,0,1));
    c->setPosition(Vector2(0.f, 0.6f));
    c->setScale(0.5, 0.1);
    c->setHorizontalAlign(HA_CENTER);
    c->setTextHorizontalAlign(HA_CENTER);

    // game over man, game over!
    //mEnergyText->setCaption("Energy: 0");
    TimeManager::getPtr()->setTimeSpeed(0.f);
    EventHandler::getDestination("OISSubsystem")->getSignal("mouseMoved")->removeAllListeners();
    dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/fail.ogg")->setGain(0.f, 1.f, 1.f);
  } else if (!gameover) {
    //mEnergyText->setCaption("Energy: "+ StringUtils::toString(playerEnergy));
    mEnergyBar->setScale(0.4 * (playerEnergy / 100.f), 0.02);
    mEnergyBar->setHorizontalAlign(HA_CENTER);
    mEnergyBarCap->setScale(0.4 * (playerEnergyCap / 100.f), 0.02);
    mEnergyBarCap->setHorizontalAlign(HA_CENTER);
    if (playerEnergy < 20.f) {
      mEnergyBarCap->setColor(Colour( ((sin(timeLived * 5) + 1.0) / 4) + 0.2,0.2,0.2, 0.5f));
    } else {
      mEnergyBarCap->setColor(Colour(0.2,0.2,0.2, 0.5f));
    }
  }

  // lololol this is terribad
  for (int i = 0; i < Powerup::kill_us.size(); ++i) {
    mEngine->getPtr()->killObject(Powerup::kill_us[i]->getID());
  }
  Powerup::kill_us.clear();
}
//---------------------------------------------------------------------------

void PlayState::deinit() {
  /*for (int i = 0; i < enemies.size(); ++i) {
    delete enemies[i];
  }
  enemies.clear();*/
  mGfx->getRootSceneNode()->removeChild(fsmesh);
  mGfx->destroySceneNode(fsmesh);
  fsmesh = NULL;
}
//---------------------------------------------------------------------------

