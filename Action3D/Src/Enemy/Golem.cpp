#include "Golem.h"

enum ANIM_ID
{
    A_RUN = 0,
    A_WAIT,
    A_ATK1,
    A_ATK2,
    A_ATK3,
};
CGolem::CGolem(const VECTOR3& pos, const float& rotY)
{
    transform.position = pos;
    transform.rotation.y = rotY;
    mesh = new CFbxMesh;
    animator = new Animator;
    mesh->Load("Data/Golem/Golem.mesh");
    animator->SetModel(mesh);
    mesh->LoadAnimation(A_RUN, "Data/Golem/golem_walk.anmx", true);
    mesh->LoadAnimation(A_WAIT, "Data/Golem/golem_stand.anmx", true);
    mesh->LoadAnimation(1, "Data/Golem/golem_stand.anmx", true);
    mesh->LoadAnimation(1, "Data/Golem/golem_stand.anmx", true);
    mesh->LoadAnimation(1, "Data/Golem/golem_stand.anmx", true);
    animator->Play(A_RUN);
    transform.scale = VECTOR3(0.3f, 0.3f, 0.3f);
}

CGolem::~CGolem()
{
}

void CGolem::Update()
{
    Object3D::Update();
}

VECTOR3 CGolem::ColldeSphere(const VECTOR3& center ,const float& radius)
{
    VECTOR3 myCenter = transform.position + VECTOR3(0, 0.5f, 0);
    float myRad = 1.0f;
    VECTOR3 dir = center - myCenter;
    float d = magnitude(dir);

    if (d < myRad + radius) {
        //押し返すベクトルを返す
        //return VECTOR3(0, 0, -1);
        //dirの向きに、めり込んだ量を返す
        return normalize(dir) * (radius + myRad - d);
    }

    return VECTOR3();
}