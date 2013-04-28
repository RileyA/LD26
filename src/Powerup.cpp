#include "Powerup.h"
#include "PlayState.h"

std::vector<Powerup*> Powerup::kill_us;

Powerup::Powerup(Vector3 pos, Signal* move) {
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
  move->addListener(createSlot("playerMoved",this,&Powerup::playerMoved));
  m_mesh = mGfx->createMesh("PowerPack.mesh");
  mGfx->getRootSceneNode()->addChild(m_mesh);
  m_mesh->setPosition(pos);
  position = pos;
  super = rand() % 10 == 0;
  if (super) {
    m_mesh->setMaterialName("PowerPackSuper");
    m_mesh->setMaterialName("PowerPackGlowSuper", 1);
    // set material TODO
  }
}

Powerup::~Powerup() {
  mGfx->getRootSceneNode()->removeChild(m_mesh);
  mGfx->destroySceneNode(m_mesh);
}

void Powerup::update(Real delta) {
  return;
}

void Powerup::playerMoved(const Message& msg) {
  bool dead = false;
  if(const MessageAny<Vector3>* ms = message_cast<Vector3>(msg))
  {
    if (ms->data.squaredDistance(position) < 2.25f) {
      if (super) {
        dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/powerup.ogg")->setGain(0.f, 1.f, 1.f);;
        PlayState::playerEnergy = PlayState::playerEnergyCap = PlayState::playerEnergyCap + 10;
      } else {
        dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/powerup2.ogg")->setGain(0.f, 1.f, 1.f);
        PlayState::playerEnergy = std::min(PlayState::playerEnergy + 50, PlayState::playerEnergyCap);
      }
      dead = true;
    }
  }
  if (dead) {
    //Engine::getPtr()->killObject(getID()); // kill ourself (essentially "delete this")
    kill_us.push_back(this);
  }
}
