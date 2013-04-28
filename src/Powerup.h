#ifndef POWERUP_H
#define POWERUP_H

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OgreSubsystem/Spline.h"
#include "BulletSubsystem/BulletSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"

class Powerup : public Object {
public:

  Powerup(Vector3 pos, Signal* move);
  ~Powerup();

  void playerMoved(const Message& m);

  void update(Real delta);

  // i am a terrible horrible horrible bad terrible person
  static std::vector<Powerup*> kill_us;

private:

  Mesh* m_mesh;
  Vector3 position;
  bool super;
  OgreSubsystem* mGfx;
  BulletSubsystem* mPhysics;

  Vector3 playerPos;

};

#endif

