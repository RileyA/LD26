#ifndef RUINS_LD_H
#define RUINS_LD_H

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OgreSubsystem/Spline.h"
#include "BulletSubsystem/BulletSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"

struct Ruin {
  String mesh;
  String phys_mesh;
  float radius;
  MeshData d;
  std::vector<std::pair<Vector3, Vector3> > spawns;
};

class Ruins {
public:

  static std::vector<Ruin*> RUINS;

  Ruins(Ruin* r, Vector3 pos, float y);
  ~Ruins();

  void deactivate();
  void reactivate();

//private: #yolo

  Mesh* m_mesh;
  Ruin* ruin;
  CollisionObject* m_phys;
  OgreSubsystem* mGfx;
  BulletSubsystem* mPhysics;

  Vector3 position;
  float yaw;

  bool active;

};

#endif
