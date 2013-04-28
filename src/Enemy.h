#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include "OryxEngine.h"
#include "OryxObject.h"
#include "OgreSubsystem/OgreSubsystem.h"
#include "OgreSubsystem/Spline.h"
#include "BulletSubsystem/BulletSubsystem.h"
#include "OISSubsystem/OISSubsystem.h"

// indestructible enemy, only one will be active at once
//  hunts the player with a spotlight and flings smaller enemies
//  at the player
class Enemy : public Object {
public:

  Enemy();
  ~Enemy();

  void update(Real delta);
  
  class MiniEnemy {
  public:
    MiniEnemy(Enemy* parent);
    ~MiniEnemy();
    void update(Real delta);
    Mesh* m_mesh;
    Mesh* m_expl;
    Enemy* m_parent;
    SceneNode* n;
    float circle;
    float speed;
    bool attack;
    Vector3 attack_dir;
    bool dead;
    bool hit;
    float explode_anim;
  };

  friend class MiniEnemy;

  enum State {
    ES_PATROL,
    ES_WAIT,
    ES_SEARCH,
    ES_COMBAT
  };

  void playerMoved(const Message& m);

private:

  void decidePatrol(bool search = false);
  void checkLOS(Real delta);

  Vector3 m_dir;

  Real los_time;

  bool sawPlayer;
  Vector3 lastKnown;

  float waitTimer;

  std::vector<Vector3> m_patrol;

  State m_state;
  Mesh* m_mesh;
  Light* light;
  BillboardSet* bs;
  std::vector<MiniEnemy*> m_minis;

  float bob;
  OgreSubsystem* mGfx;
  BulletSubsystem* mPhysics;
  Vector3 m_pos;

  Spline* m_path;
  float path_len;
  float path_pos;

  Vector3 playerPos;

  float ammo_timer;

};

#endif
