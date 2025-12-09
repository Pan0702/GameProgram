#include "Golem.h"

#include "EnemyManager.h"
#include "../MyLib.h"
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
    teritoriCenter = pos;
    action = ACT_STAND;
    intent = INT_WALK;
    animator->Play(A_IDLE);
    deadTimer = 0.0f;
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

bool CGolem::CollideSword(const VECTOR3& top, const VECTOR3& btm)
{
    if (CollideSegmentToSphere(top, btm, transform.position + VECTOR3(0, 1, 0), 1))
    {
        ChangeIntent(INT_DEAD);
    }
    return false;
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
    case INT_BACK:
        IntBack();
        break;
    case INT_DEAD:
        IntDead();
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
        ChangeAction(ACT_CHASE);
        break;
    case INT_BACK:
        ChangeAction(ACT_STAND);
        break;
    case INT_DEAD:
        ChangeAction(ACT_DEAD);
    default:
        break;
    }
    intent = inte;
}

bool CGolem::InSight(const VECTOR3& pos, const float& dist, const float& angle) const
{
    VECTOR3 toPlayer = pos - transform.position;
    VECTOR3 forward = VECTOR3(0, 0, 1) * XMMatrixRotationY(transform.rotation.y);
    VECTOR3 toPlayerNorm = normalize(toPlayer);
    if (dot(normalize(forward), toPlayerNorm) > cosf(angle) && magnitude(toPlayer) < dist)
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
    if (not InSight(pl->GetTransform().position, 6.0f, 40.0f * DegToRad))
    {
        ChangeIntent(INT_BACK);
    }
    float v = magnitude(transform.position - teritoriCenter);
    if (v > 10.0f)
    {
        ChangeIntent(INT_BACK);
    }
}

void CGolem::IntBack()
{
    
    animator->Play(A_WALK);
    animator->SetPlaySpeed(1.0f);
    float RotSpeed = 20.0f * DegToRad; //回転のはやさ
    float& lotY = transform.rotation.y;
    VECTOR3 velocity = VECTOR3(teritoriCenter.x, 0, teritoriCenter.z) - VECTOR3(transform.position.x, 0, transform.position.z);
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
    transform.position += move * 0.02f;
    VECTOR3 v = teritoriCenter - transform.position;
    if (magnitude(v) < 1.0f)
    {
        ChangeIntent(INT_WALK);
        animator->Play(A_IDLE);
        animator->SetPlaySpeed(1.0f);
    }
}

void CGolem::IntDead()
{
 

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
    case ACT_DEAD:
        ActDead();
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
    case ACT_DEAD:
        animator->Play(A_DEATH);
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
    if (magnitude(plPos - transform.position) < 2.0f)
    {
        CEnemyManager* man = ObjectManager::FindGameObject<CEnemyManager>();
        if (man->CanAtk(this))
        {
            ChangeAction(ACT_PUNCH);
        }
    }else
    {
        transform.position += move * 0.05f;
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

void CGolem::ActDead()
{
    float f = animator->CurrentFrame();
    if (f >= 50.0f) animator->SetPlaySpeed(f / 50.0f);
    if (animator->Finished())
    {
        deadTimer += 1.0f;
        if (deadTimer > 30.0f)
        {
            transform.position.y -= 0.01f;
            if (transform.position.y < -1.0f)
            DestroyMe();
        }
    }
}
