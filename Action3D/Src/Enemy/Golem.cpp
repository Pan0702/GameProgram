#include "Golem.h"

#include "../Player/Player.h"

class CPlayer;

enum ANIM_ID
{
    A_IDLE = 0,
    A_WALK,
    A_ATK1,
    A_DEATH,
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
    mesh->LoadAnimation(A_WALK, "Data/Golem/golem_walk.anmx", true);
    mesh->LoadAnimation(A_IDLE, "Data/Golem/golem_stand.anmx", true);
    mesh->LoadAnimation(A_ATK1, "Data/Golem/golem_attack.anmx", false);
    mesh->LoadAnimation(A_DEATH, "Data/Golem/golem_die.anmx", false);
    animator->Play(A_WALK);
    transform.scale = VECTOR3(0.3f, 0.3f, 0.3f);
    action = ACT_CHASE;
}

CGolem::~CGolem()
{
}

void CGolem::Update()
{
    Object3D::Update();
    UpdateAction();
}

VECTOR3 CGolem::ColldeSphere(const VECTOR3& center, const float& radius)
{
    VECTOR3 myCenter = transform.position + VECTOR3(0, 0.5f, 0);
    float myRad = 1.0f;
    VECTOR3 dir = center - myCenter;
    float d = magnitude(dir);

    if (d < myRad + radius)
    {
        //押し返すベクトルを返す
        //return VECTOR3(0, 0, -1);
        //dirの向きに、めり込んだ量を返す
        return normalize(dir) * (radius + myRad - d);
    }

    return VECTOR3();
}

void CGolem::UpdateAction()
{
    switch (action)
    {
    case ACT_CHASE:
        ActChase();
        break;
    case ACT_PUNCH:
        ActPunch();
        break;
    }
}

void CGolem::ActChase()
{
   CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    VECTOR3 plPos = pl->GetTransform().position;
    VECTOR3 velocity = VECTOR3(plPos.x, 0, plPos.z) - VECTOR3(transform.position.x, 0, transform.position.z);
    if (magnitude(velocity) > 0)
    {
    
        XMMATRIX mat = XMMatrixRotationY(transform.rotation.y);
        VECTOR3 foward = VECTOR3(0, 0, 1) * mat;
        VECTOR3 up = VECTOR3(0, 1, 0) * mat;
        float ip2 = dot(foward, normalize(velocity));

        //30.0f * DegToRadが定数だからコンパイル時に計算済みになって処理負荷が減る
        if (ip2 >= cos(30.0f * DegToRad))
        {
            transform.rotation.y = atan2f(velocity.x, velocity.z);
        }
        else
        {
            VECTOR3 right = VECTOR3(1, 0, 0) * mat;
            float ip = dot(right, velocity);
            if (ip > 0)
            {
                transform.rotation.y += 20.0f * DegToRad;
            }
            else
            {
                transform.rotation.y -= 20.0f * DegToRad;
            }
        }
        
        transform.position += (velocity * 0.005f);
        animator->MergePlay(A_WALK);
    }
}

void CGolem::ActPunch()
{
}
