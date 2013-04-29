#include "Enemy.h"
#include "PlayState.h"

Enemy::Enemy(Vector3 startpos) {
  bob = 0;
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
  m_mesh = mGfx->createMesh("Enemy.mesh");
  for (int i = 0; i < 5; ++i) {
    m_minis.push_back(new MiniEnemy(this));
  }
  bs = mGfx->createBillboardSet("AUTO_NAME_ME");
  bs->addBillboard(Vector3(0,0,0), 10, 10);
  bs->setMaterialName("enemy_glow");
  mGfx->getRootSceneNode()->addChild(m_mesh);
  m_mesh->addChild(bs);
  light = mGfx->createLight();
  m_mesh->addChild(light);
  light->addChild(mGfx->createMesh("SpotBeam.mesh"));
  light->setDirection(Vector3(0,-1,0));
  light->pitch(20.0);

  m_dir = Vector3(0,0,-1);
  startpos.y = 20;
  m_pos = startpos;

  m_state = ES_WAIT;
  waitTimer = 10.f;
  m_path = new Spline();
  createSlot("playerMoved",this,&Enemy::playerMoved);
  los_time = 0;
  ammo_timer = 0.f;
}

Enemy::~Enemy() {
  // TODO
  delete m_path;
  for (int i = 0; i < m_minis.size(); ++i) {
    delete m_minis[i];
  }
  m_minis.clear();
  mGfx->getRootSceneNode()->removeChild(m_mesh);
  m_mesh->removeChild(light);
  m_mesh->removeChild(bs);
  mGfx->destroySceneNode(m_mesh);
  mGfx->destroySceneNode(bs);
  mGfx->destroySceneNode(light);
}

void Enemy::playerMoved(const Message& msg) {
  // WALLHAX!!11
  if(const MessageAny<Vector3>* ms = message_cast<Vector3>(msg))
  {
    playerPos = ms->data;
  }
}

void Enemy::update(Real delta) {
  bob += delta;

  if (m_minis.size() < 5) {
    ammo_timer += delta;
    if (ammo_timer >= 5.f) {
      ammo_timer = 0.f;
      m_minis.push_back(new MiniEnemy(this));
      if (m_minis.size() >= 2) {
        MiniEnemy* tmp = m_minis[m_minis.size() - 1];
        m_minis[m_minis.size() - 1] = m_minis[m_minis.size() - 2];
        m_minis[m_minis.size() - 2] = tmp;
      }
    }
  }

  checkLOS(delta);

  Quaternion q;
  float t;
  Vector3 cur;
  float speed_factor;
  switch(m_state) {
    case ES_WAIT:
      waitTimer -= delta;
      if (waitTimer < 0.f) {
        m_state = ES_PATROL;
        decidePatrol();
        //std::cout<<"PATROL!\n";
      }
      break;
    case ES_PATROL:
      cur = m_pos;
      cur.y = 0;
      speed_factor = playerPos.distance(m_pos);
      speed_factor /= 100.f;
      speed_factor = std::max(speed_factor, 1.f);

      path_pos += delta * 10 * speed_factor;
      t = path_pos / path_len;

      if (t >= 1.f) {
        waitTimer = 3.f;
        m_state = ES_WAIT;
        m_patrol.clear();
      } else {
        Vector3 next = m_path->interpolate(t);
        Vector3 dir = next - cur;
        dir.normalize();
        next.y = m_pos.y;
        m_pos = next;
        //q.fromAxes(dir.crossProduct(Vector3::UNIT_Y), Vector3::UNIT_Y, dir * -1);
        //m_mesh->setOrientation(q);
        m_dir = dir;
      }
      break;
    case ES_COMBAT:
      // track the player
      Vector3 target = playerPos;
      Vector3 pos = m_pos;
      Vector3 dir = target - m_pos;
      m_dir = dir;

      Vector3 ldir = light->getAbsoluteOrientation() * Vector3::NEGATIVE_UNIT_Y;
      ldir.y = 0;
      ldir.normalize();

      float pd = pos.distance(target);

      ldir *= pd;

      Vector3 motion = ldir - (playerPos - m_pos);
      motion.y = 0;
      float dd = motion.normalize();
      motion *= std::min(delta * 4, dd);
      m_pos += motion;


      //q.fromAxes(dir.crossProduct(Vector3::UNIT_Y), Vector3::UNIT_Y, dir * -1);
      //m_mesh->setOrientation(q);

      break;
    //case ES_SEARCH:
      // snoop around...
    //  break;
  };

  RaycastReport r = mPhysics->raycast(m_pos, Vector3::NEGATIVE_UNIT_Y, 50.f, 0, 0);

  if (r.hit) {
    float h = r.position.distance(m_pos);
    if (h < 30.f) {
      m_pos.y += delta;
    } else if(h > 32.f) {
      m_pos.y -= delta;
    }
  }

  // dir
  Vector3 cdir = m_mesh->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
  cdir.y = 0;
  m_dir.y = 0;
  cdir.normalize();
  m_dir.normalize();
  float a = cdir.angleBetween2(m_dir);

  if (Vector3(0,1,0).dotProduct(cdir.crossProduct(m_dir)) < 0) {
    a *= -1;
  }

  if (a < 0.f) {
    a = std::max(a, delta * -200.f);
  } else {
    a = std::min(a, delta * 200.f);
  }

  m_mesh->yaw(a);

  m_mesh->setPosition(m_pos + Vector3(0,sin(bob * 4) * 0.5f,0));

  for (int i = 0; i < m_minis.size(); ++i) {
    m_minis[i]->update(delta);
  }

  if (!m_minis.empty() && m_minis.back()->dead) {
    delete m_minis.back();
    m_minis.pop_back();
  }
}

Enemy::MiniEnemy::MiniEnemy(Enemy* parent) {
  m_parent = parent;
  m_mesh = parent->mGfx->createMesh("MiniEnemy.mesh");
  n = parent->mGfx->createSceneNode();
  parent->m_mesh->addChild(n);
  n->addChild(m_mesh);
  n->yaw(Rand::get().genFloat(0.f, 180.f));
  n->roll(Rand::get().genFloat(0.f, 180.f));
  m_mesh->setPosition(Vector3(2.5,0,0));
  circle = Rand::get().genFloat(0.f, 6.f);
  speed = Rand::get().genFloat(4.f, 5.f);
  attack = false;
  dead = false;
  hit = false;
}

Enemy::MiniEnemy::~MiniEnemy() {
  // TODO
}

void Enemy::decidePatrol(bool search) {
  delete m_path;
  m_path = new Spline();
  Vector3 prev = m_pos;
  prev.y = 0.f;
  path_len = 0.f;
  path_pos = 0.001f;

  m_path->addPoint(prev);
  if (search) {
    prev = playerPos;
    prev.y = 0;
    m_path->addPoint(prev);
    prev += (playerPos - m_pos);
    m_path->addPoint(prev);
  }

  float speed_factor = playerPos.distance(m_pos);
  speed_factor /= 100.f;
  speed_factor = std::max(speed_factor, 1.f);

  for (int i = 0; i < 6; ++i) {

    Vector3 pdir = playerPos - m_pos;
    pdir.y = 0;
    pdir.normalize();
    Quaternion q;
    q.FromAngleAxis(Rand::get().genFloat(-1.0, 1.0), Vector3::UNIT_Y);
    pdir = q * pdir;
    //pdir.randomDeviant(0.75f, Vector3::UNIT_Y);
    pdir.y = 0;
    pdir.normalize();

    pdir *= Rand::get().genFloat(30.f, 50.f) * speed_factor;
    m_patrol.push_back(prev + pdir);

    //m_patrol.push_back(prev + Vector3(Rand::get().genFloat(-50.f, 50.f),
    //  0.f, Rand::get().genFloat(-50.f, 50.f)));
    path_len += prev.distance(m_patrol.back());
    prev = m_patrol.back();
    m_path->addPoint(m_patrol.back());
  }
  m_path->recalc();
  //std::cout<<"Path len: "<<path_len<<"\n";
}

void Enemy::checkLOS(Real delta) {
  Vector3 target = playerPos;

  Vector3 origin = m_pos + Vector3(0,sin(bob * 4) * 0.5f,0);
  Vector3 dir = target - origin;
  dir.normalize();
  Vector3 tmp = Vector3::NEGATIVE_UNIT_Y;
  tmp = light->getAbsoluteOrientation() * tmp;

  if (tmp.angleBetween(dir) < 34.f) {
    RaycastReport r = mPhysics->raycast(origin, dir, 5000.f, 0, 0);
    if (r.hit && r.group == 0x1) {
      lastKnown = playerPos;
      los_time += delta;
      if (los_time > 0.1f && m_state != ES_COMBAT) {
        m_state = ES_COMBAT;
        //std::cout<<"I SEE U\n";
        los_time = 0;
      } else if (los_time > 1.1f && m_state == ES_COMBAT && m_minis.size() > 0 && !m_minis.back()->attack) {
        //std::cout<<"ATTAAAACCKKK\n";

      dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/attack.ogg")->setGain(0.f, 1.f, 1.f);

        m_minis.back()->attack = true;
        Vector3 wpos = m_minis.back()->m_mesh->getAbsolutePosition();
        m_mesh->removeChild(m_minis.back()->n);
        m_minis.back()->n->removeChild(m_minis.back()->m_mesh);
        mGfx->getRootSceneNode()->addChild(m_minis.back()->m_mesh);
        mGfx->destroySceneNode(m_minis.back()->n);
        m_minis.back()->m_mesh->setPosition(wpos);
        Vector3 ad = playerPos - wpos;
        ad.normalize();
        m_minis.back()->attack_dir = ad;
        los_time = 0;
      }
    } else {
      los_time -= delta / 2.f;
    }

  } else {
    los_time -= delta / 2.f;
  }

  if (los_time < 0.f && m_state != ES_COMBAT) {
    los_time = 0.f;
  }
  if (los_time > 1.2f) {
    los_time = 1.2f;
  }
      //printf("%f.\n", los_time);
  if (los_time < -0.1 && m_state == ES_COMBAT) {
    //std::cout<<"WHERE'D HE GOOOOO???\n";
    decidePatrol(true);
    m_state = ES_PATROL;
  }
}

void Enemy::MiniEnemy::update(Real delta) {
  // todo orbit parent
  if (!attack) {
    circle += delta * speed;
    m_mesh->setPosition(Vector3(cos(circle) * 2.5f, sin(circle) * 2.5f,0));
  } else if (!hit) {
    float dist = delta * 40;
    RaycastReport r = m_parent->mPhysics->raycast(m_mesh->getPosition(), attack_dir, dist, 0, 0);
    if (r.hit) {
      //dead = true;
      hit = true;
      explode_anim = 1.f;
      m_mesh->setPosition(r.position);
      m_expl = m_parent->mGfx->createMesh("explode.mesh");
      m_expl->setPosition(r.position);
      m_parent->mGfx->getRootSceneNode()->addChild(m_expl);
      dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D("../media/audio/explode.ogg")->setGain(0.f, 1.f, 1.f);

    } else {
      m_mesh->setPosition(m_mesh->getPosition() + attack_dir * dist);
    }
  } else if (!dead) {
    // TODO explosion effect, sfx, etc
    explode_anim += delta * 10;
    if (m_mesh) {
      //m_parent->mGfx->getRootSceneNode()->removeChild(m_mesh);
      //m_parent->mGfx->destroySceneNode(m_mesh);
      //m_mesh = NULL;
    }
    float s = explode_anim * explode_anim;
    m_expl->setScale(Vector3(s,s,s));

    Real dst = m_parent->playerPos.distance(m_expl->getPosition());
    if (dst < explode_anim * explode_anim) {
      PlayState::playerEnergy -= ((explode_anim * explode_anim) - dst) / 8.9f;
    }

    if (explode_anim > 3.9f) {
      //m_parent->mGfx->getRootSceneNode()->removeChild(m_expl);
      //m_parent->mGfx->destroySceneNode(m_expl);
      //m_expl = NULL;
    }
  }
}


