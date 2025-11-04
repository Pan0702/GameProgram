#include "Golem.h"

CGolem::CGolem(const VECTOR3& pos,const float& rotY)
{
    transform.position = pos;
    transform.rotation.y = rotY;
    mesh = new CFbxMesh;
    animator = new Animator;
    mesh->Load("Data/Golem/Golem.mesh");
    animator->SetModel(mesh);
    mesh->LoadAnimation(1,"Data/Golem/golem_stand.anmx",true);
}

CGolem::~CGolem()
{
}

void CGolem::Update()
{
    Object3D::Update();
}
