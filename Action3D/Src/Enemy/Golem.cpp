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

    transform.scale = VECTOR3(0.3f, 0.3f, 0.3f);
    action = ACT_STAND;
    intent = INT_WALK;
    animator->Play(A_IDLE);
}

CGolem::~CGolem()
{
}

void CGolem::Update()
{
    Object3D::Update();
    UpdateIntention();
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

void CGolem::UpdateIntention()
{
    switch (intent)
    {
    case INT_WALK:
        IntWalk();
        break;
    case INT_ATK:
        IntAtk();
        break;
    default:
        break;
    }
}

void CGolem::ChangeIntent(Intent inte)
{
    if (intent == inte) return;
    switch (inte)
    {
    case INT_WALK:
        ChangeAction(ACT_STAND);
        break;
    case INT_ATK:
        ChangeAction(ACT_PUNCH);
        break;
    default:
        break;
    }
    intent = inte;
}
bool CGolem::InSight(const VECTOR3& pos, const float&disit,const float& angle)
{
    VECTOR3 toPlayer = pos - transform.position;
    VECTOR3 forward = VECTOR3(0, 0, 1) * XMMatrixRotationY(transform.rotation.y);
    VECTOR3 toPlayerNorm = normalize(toPlayer);
    if (dot(normalize(forward), toPlayerNorm ) > cosf(angle) && magnitude(toPlayer) < disit)
    {
            return true;
    }
    return false;
}
void CGolem::IntWalk()
{

    CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    if (InSight(pl->GetTransform().position, 5.0f, 20.0f * DegToRad))
    {
        ChangeIntent(INT_ATK);
    }

}

void CGolem::IntAtk()
{
    CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    VECTOR3 toPlayer = pl->GetTransform().position - transform.position;
    if (magnitude(toPlayer) > 5.0f)
    {
        ChangeIntent(INT_WALK);
    }
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
    case ACT_STAND:
        ActStand();
        break;
    }
}

void CGolem::ChangeAction(Action act)
{
    if (action == act)
        return;
    switch (act)
    {
    case ACT_PUNCH:
        animator->Play(A_ATK1);
        animator->SetPlaySpeed(1.0f);
        break;
    case ACT_CHASE:
        animator->Play(A_WALK);
        animator->SetPlaySpeed(5.0f);
        break;
    case ACT_STAND:
        animator->Play(A_IDLE);
        animator->SetPlaySpeed(1.0f);
        break;
    default:
        break;
    }
    action = act;
}

void CGolem::ActChase()
{
    float RotSpeed = 20.0f * DegToRad; //回転のはやさ
    float& lotY = transform.rotation.y;
    CPlayer* pl = ObjectManager::FindGameObject<CPlayer>();
    VECTOR3 plPos = pl->GetTransform().position;
    VECTOR3 velocity = VECTOR3(plPos.x, 0, plPos.z) - VECTOR3(transform.position.x, 0, transform.position.z);
    XMMATRIX mat = XMMatrixRotationY(lotY);
    VECTOR3 foward = transform.forward();
    float ip2 = dot(foward, normalize(velocity));

    //30.0f * DegToRadが定数だからコンパイル時に計算済みになって処理負荷が減る
    if (dot(foward, normalize(velocity)) > cosf(RotSpeed))
    {
        lotY = atan2f(velocity.x, velocity.z);
    }
    else
    {
        VECTOR3 right = VECTOR3(1, 0, 0) * mat;
        if (dot(right, velocity) > 0)
        {
            lotY += RotSpeed;
        }
        else
        {
            lotY -= RotSpeed;
        }
    }
    VECTOR3 move = VECTOR3(0, 0, 1) * XMMatrixRotationY(lotY);
    transform.position += move * 0.05f;
    if ((plPos - transform.position).Length() < 2.0f)
    {
        ChangeAction(ACT_PUNCH);
    }
}

void CGolem::ActPunch()
{
    if (animator->Finished())
    {
        ChangeAction(ACT_CHASE);
    }
}

void CGolem::ActStand()
{
}
