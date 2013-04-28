#include "Ruins.h"

std::vector<Ruin*> Ruins::RUINS = std::vector<Ruin*>();

Ruins::Ruins(Ruin* r, Vector3 pos, float y) {
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
  yaw = y;
  position = pos;
  active = false;
  ruin = r;
  reactivate();
}

Ruins::~Ruins() {
  deactivate();
}

void Ruins::deactivate() {
  if (active) {
    m_phys->_kill();
    mGfx->getRootSceneNode()->removeChild(m_mesh);
    mGfx->destroySceneNode(m_mesh);
    m_phys->_kill();
    m_phys = NULL;
    active = false;
  }
}

void Ruins::reactivate() {
  if (!active) {
    active = true;
    m_mesh = mGfx->createMesh(ruin->mesh);
    m_mesh->setPosition(position);
    m_mesh->yaw(yaw);
    m_phys = dynamic_cast<CollisionObject*>(mPhysics->createStaticTrimesh(
      ruin->d, Vector3::ZERO, ruin->phys_mesh));
    mGfx->getRootSceneNode()->addChild(m_mesh);
    m_phys->setOrientation(m_mesh->getOrientation());
    m_phys->setPosition(m_mesh->getPosition());
    m_phys->setCollisionGroup(0x4);
    m_phys->setCollisionMask(0xffff);
  }
}

