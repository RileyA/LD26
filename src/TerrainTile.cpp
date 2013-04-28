#include "TerrainTile.h"
#include <libnoise/noise.h>
#include "Powerup.h"


const double TerrainTile::SIZE_X = 100.0;
const double TerrainTile::SIZE_Z = 100.0;
//const int32_t TerrainTile::VERTS_X = 90;
//const int32_t TerrainTile::VERTS_Z = 90;


TerrainTile::TerrainTile(iVec2 position, noise::module::Perlin* p, Signal* pmove)
  :m_position(position) {
  m_perlin = p;
  playerMv = pmove;
  m_active = false;
  Vector3 pos(position.x * SIZE_X, 0.f, position.y * SIZE_Z);
  Vector3 corner_pos(position.x * SIZE_X - SIZE_X / 2.0, 0.f, 
    position.y * SIZE_Z - SIZE_Z / 2.0);
	mGfx = dynamic_cast<OgreSubsystem*>(Engine::getPtr()->getSubsystem("OgreSubsystem"));
	mPhysics = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));

  // generate noise...
  memset(heights, 0, sizeof(heights));
  for (int z = -1; z < VERTS_Z + 2; ++z) {
    for (int x = -1; x < VERTS_X + 2; ++x) {
      heights[1 + x + (VERTS_X + 3) * (z + 1)] = m_perlin->GetValue(
        static_cast<double>(pos.x + x * (SIZE_X / VERTS_X)) * 0.0035,
        static_cast<double>(0.0),
        static_cast<double>(pos.z + z * (SIZE_Z / VERTS_Z)) * 0.0035
        );

      /*float h = heights[1 + x + (VERTS_X + 3) * (z + 1)];
      if (h < 0.1f && h > -0.1f) {
        h /= 1 + (10 - fabs(h) * 50);
      }
      heights[1 + x + (VERTS_X + 3) * (z + 1)] = h;*/
    }
  }

  int num_rois = Rand::get().gen(0, 100) - 98;
  std::vector<ROI> rois;
  for (int i = 0; i < num_rois; ++i) {
    ROI r;

    Ruin* ruin = Ruins::RUINS[Rand::get().gen() % Ruins::RUINS.size()];

    r.radius = ruin->radius * 0.75;
    r.x = Rand::get().genFloat(r.radius * 1.5f, SIZE_X - r.radius * 1.5f);
    r.z = Rand::get().genFloat(r.radius * 1.5f, SIZE_Z - r.radius * 1.5f);

    bool skip = false;
    for (int i = 0; i < rois.size(); ++i) {
      float dx = r.x - rois[i].x;
      float dz = r.z - rois[i].z;
      if (dx* dx + dz * dz < (r.radius + rois[i].radius) * (r.radius + rois[i].radius)) {
        skip = true;
      }
    }
    if (skip) break;
    rois.push_back(r);

    Vector3 smoothed_pos = corner_pos + Vector3(r.x, smoothROI(r) * 20 + 0.01f,r.z);
    Ruins* rns = new Ruins(ruin, smoothed_pos,
      Rand::get().genFloat(1.f, 360.f));


    Vector3 smp = rns->m_mesh->getPosition();

    // aaand generate some powerups!
    if (!ruin->spawns.empty()) {
      float odds[] = {0.95, 0.35, 0.05};
      for (int j = 0; j < ruin->spawns.size(); ++j) {
        if (Rand::get().genFloat(0.f, 1.f) < odds[j]) {
          Vector3 disperse = ruin->spawns[j].second;
          Vector3 spawnPos = ruin->spawns[j].first + 
            Vector3(Rand::get().genFloat(-disperse.x, disperse.x), 0.f,
              Rand::get().genFloat(-disperse.z, disperse.z));
          Powerup* pw = new Powerup(smp + rns->m_mesh->getOrientation() * spawnPos, playerMv);
        }
      }
    } else {
      Powerup* pw = new Powerup(smoothed_pos + Vector3(0,2.5,0), playerMv);
    }

    m_ruins.push_back(rns);
  }
}

TerrainTile::~TerrainTile() {
  deactivate(); 
}

void TerrainTile::makeMesh() {
    MeshData d;

    iVec2 pos = m_position;

    Vector3 center_pos(pos.x * SIZE_X, 0.f, pos.y * SIZE_Z);
    Vector3 corner_pos(pos.x * SIZE_X - SIZE_X / 2.0, 0.f, 
      pos.y * SIZE_Z - SIZE_Z / 2.0);

    Vector3 offset(SIZE_X / -2.0, 0.f, SIZE_Z / -2.0);

    double step_x = SIZE_X / VERTS_X;
    double step_z = SIZE_Z / VERTS_Z;

    // create grid of vertices..
    for (int z = 0; z <= VERTS_Z; ++z) {
      for (int x = 0; x <= VERTS_X; ++x) {

        Vector3 lpos = Vector3(x * step_x, -2, z * step_z);

        Vector3 abs_pos = corner_pos + lpos;
        float height = sampleNoise(x, z);
        float hx1 = sampleNoise(x - 1, z);
        float hx2 = sampleNoise(x + 1, z);
        float hz1 = sampleNoise(x, z - 1);
        float hz2 = sampleNoise(x, z + 1);
        
        float slopex = hx2 - hx1;
        float slopez = hz2 - hz1;

        float heightfactor = 20;

        Vector3 normal(slopex * heightfactor,
          1.f * (SIZE_X / VERTS_X), 
          slopez * heightfactor);
        normal.normalize();
        d.normals.push_back(normal.x);
        d.normals.push_back(normal.y);
        d.normals.push_back(normal.z);

        lpos.y = height * heightfactor;
        d.vertex(lpos + offset);
      }
    }

    // add indices
    for (int z = 0; z <= VERTS_Z - 1; ++z) {
      for (int x = 0; x <= VERTS_X - 1; ++x) {
        uint32_t v1 = x + z * (VERTS_X + 1);
        uint32_t v2 = (x + 1) + z * (VERTS_X + 1);
        uint32_t v3 = x + (z + 1) * (VERTS_X + 1);
        uint32_t v4 = (x + 1) + (z + 1) * (VERTS_X + 1);
        d.indices.push_back(v1);
        d.indices.push_back(v3);
        d.indices.push_back(v2);
        d.indices.push_back(v2);
        d.indices.push_back(v3);
        d.indices.push_back(v4);
      }
    }

    m_mesh = mGfx->createMesh(d);
    mGfx->getRootSceneNode()->addChild(m_mesh);
    m_mesh->setMaterialName("Terrain");
    m_mesh->setPosition(center_pos);

    m_phys = mPhysics->createStaticTrimesh(d, center_pos);
    CollisionObject* obj = dynamic_cast<CollisionObject*>(m_phys);
    obj->setCollisionGroup(0x2);
    obj->setCollisionMask(0xffff);
}

void TerrainTile::deactivate() {
  if (m_active) {
    mGfx->getRootSceneNode()->removeChild(m_mesh);
    mGfx->destroySceneNode(m_mesh);
    m_phys->_kill();
    m_phys = NULL;
    m_mesh = NULL;
    m_active = false;
    for (int i = 0; i < m_ruins.size(); ++i) {
      m_ruins[i]->deactivate();
    }
  }
}

void TerrainTile::reactivate() {
  if (!m_active) {
    makeMesh();
    m_active = true;
    for (int i = 0; i < m_ruins.size(); ++i) {
      m_ruins[i]->reactivate();
    }
  }
}

float TerrainTile::smoothROI(ROI r) {
  int32_t x = 1 + r.x * (VERTS_X / SIZE_X);
  int32_t z = 1 + r.z * (VERTS_Z / SIZE_Z);

  float r2 = r.radius * r.radius;

  float avg = 0.f;
  int samples = 0;

  for (int z = -1; z < VERTS_Z + 2; ++z) {
    for (int x = -1; x < VERTS_X + 2; ++x) {
      float real_x = x * SIZE_X / VERTS_X - r.x;
      float real_y = z * SIZE_Z / VERTS_Z - r.z;
      if (real_x * real_x + real_y * real_y <= r2) {
        avg += heights[1 + x + (VERTS_X + 3) * (z + 1)];
        ++samples;
      }
    }
  }

  avg /= samples;
  //avg = 2.f;

  for (int z = -1; z < VERTS_Z + 2; ++z) {
    for (int x = -1; x < VERTS_X + 2; ++x) {
      float real_x = x * SIZE_X / VERTS_X - r.x;
      float real_y = z * SIZE_Z / VERTS_Z - r.z;
      float d = sqrt(real_x * real_x + real_y * real_y);
      if (d < r.radius * 1.5f) {
        float h = heights[1 + x + (VERTS_X + 3) * (z + 1)];

        if (d < r.radius) {
          h = avg;// + (0.5f) * h;
        } else {
          d -= r.radius;
          d /= r.radius * 0.5f;
          float interp = d;
          h = (1.f - interp) * avg + (interp) * h;
        }
        heights[1 + x + (VERTS_X + 3) * (z + 1)] = h;// + Rand::get().genFloat(-0.005f, 0.005f);
      }
    }
  }

  return avg;

}


