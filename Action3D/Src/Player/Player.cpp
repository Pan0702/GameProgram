#include "Player.h"

#include "../Enemy/Golem.h"

//#include "PlayScene.h"

class CGolem;
class PlayScene;

enum ANIM_ID
{
    A_RUN = 0,
    A_WAIT,
    A_ATTACK1,
    A_ATTACK2,
    A_ATTACK3,
};

CPlayer::CPlayer()
{
    mesh = new CFbxMesh();
    animator = new Animator();
    mesh->Load("Data/Player/PlayerChara.mesh");
    animator->SetModel(mesh);
    mesh->LoadAnimation(A_RUN, "Data/Player/Run.anmx", true);
    mesh->LoadAnimation(A_WAIT, "Data/Player/Idle.anmx", true);
    mesh->LoadAnimation(A_ATTACK1, "Data/Player/attack1.anmx", false);
    mesh->LoadAnimation(A_ATTACK2, "Data/Player/attack2.anmx", false);
    mesh->LoadAnimation(A_ATTACK3, "Data/Player/attack3.anmx", false);

    animator->Play(A_RUN);
    ChangeState(ST_NORMAL);
}

CPlayer::~CPlayer()
{
}


inline float CalcVector2Angle(const VECTOR3& vNorm1, const VECTOR3& vNorm2)
{
    float angle = acosf(vNorm1.x * vNorm2.x + vNorm1.y * vNorm2.y + vNorm1.z * vNorm2.z);
    return angle * RadToDeg;
}

void CPlayer::Update()
{
    switch (m_state)
    {
    case ST_NORMAL:
        UpdateNormal();
        break;
    case ST_ATK1:
        UpdateAtk1();
        break;
    case ST_ATK2:
        UpdateAtk2();
        break;
    case ST_ATK3:
        UpdateAtk3();
        break;
    }
    Object3D::Update();
}

void CPlayer::Draw()
{
    
    Object3D::Draw();
    CSprite spr;
    spr.DrawLine3D(VECTOR3(0,0,0),VECTOR3(0,5,0),0xffffff);
}

void CPlayer::UpdateNormal()
{
    VECTOR2 stick = LSticVec();
    VECTOR3 camVec = GameDevice()->m_vLookatPt - GameDevice()->m_vEyePt;
    float camAngle = atan2f(camVec.x, camVec.z);
    VECTOR3 velocity = VECTOR3(stick.x, 0, -stick.y);
    if (magnitude(velocity) > 0)
    {
        velocity = velocity * XMMatrixRotationY(camAngle);
#if false
        float ang = atan2f(velocity.x, velocity.z);
        float diff = ang - transform.rotation.y;
        while (diff < -180.0f) diff += 360.0f * DegToRad;
        while (diff > 180.0f) diff -= 360.0f * DegToRad;
        if (diff >= -30.0f * DegToRad && diff <= 30.0f * DegToRad)
        {
            transform.rotation.y = ang;
        }
        else if (diff > 0)
        {
            transform.rotation.y += 30.0f * DegToRad;
        }
        else if (diff < 0)
        {
            transform.rotation.y -= 30.0f * DegToRad;
        }
#else

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

#endif
        transform.position += (velocity * 0.1f);
        CGolem* gom = ObjectManager::FindGameObject<CGolem>();
            transform.position += gom->ColldeSphere(transform.position + VECTOR3(0, 0.5f, 0), 0.5f);
        animator->MergePlay(A_RUN);
    }
    else
    {
        animator->MergePlay(A_WAIT);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_M))
    {
        animator->Play(A_ATTACK1);
        ChangeState(ST_ATK1);
    }
}

void CPlayer::UpdateAtk1()
{
    if (animator->CurrentFrame() < 50.0f)
    {
        animator->SetPlaySpeed(2.0f);
    }
    else
    {
        animator->SetPlaySpeed(1.2f);
    }
    if (animator->CurrentFrame() <= 70.0f)
    {
        if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_M))
        {
            attackPushed = true;
        }
    }
    else
    {
        if (attackPushed)
        {
            animator->SetPlaySpeed(1.0f);
            animator->MergePlay(A_ATTACK2);
            ChangeState(ST_ATK2);
            attackPushed = false;
        }
    }

    if (animator->Finished())
    {
        animator->MergePlay(A_RUN);
        ChangeState(ST_NORMAL);
    }
}

void CPlayer::UpdateAtk2()
{
    if (animator->CurrentFrame() < 50.0f)
    {
        animator->SetPlaySpeed(2.0f);
    }
    else
    {
        animator->SetPlaySpeed(1.2f);
    }
    if (animator->CurrentFrame() <= 70.0f)
    {
        if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_M))
        {
            attackPushed = true;
        }
    }
    else
    {
        if (attackPushed)
        {
            animator->SetPlaySpeed(1.0f);
            animator->MergePlay(A_ATTACK3);
            ChangeState(ST_ATK3);
            attackPushed = false;
        }
    }

    if (animator->Finished())
    {
        animator->MergePlay(A_RUN);
        ChangeState(ST_NORMAL);
    }
}

void CPlayer::UpdateAtk3()
{
    if (animator->Finished())
    {
        animator->MergePlay(A_RUN);
        ChangeState(ST_NORMAL);
    }
}

void CPlayer::ChangeState(State s)
{
    m_state = s;
}


VECTOR2 CPlayer::LSticVec()
{
    auto ret = VECTOR2(0, 0);
    auto inp = GameDevice()->m_pDI->GetJoyState();
    float x = inp.lX;
    float y = inp.lY;
    ret.x = x / 1000.0f;
    ret.y = y / 1000.0f;

    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_D))
    {
        ret.x = 1.0f;
    }
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_A))
    {
        ret.x = -1.0f;
    }
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_S))
    {
        ret.y = 1.0f;
    }
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_W))
    {
        ret.y = -1.0f;
    }
    if (magnitude(ret) > 1.0f)
    {
        ret = normalize(ret);
    }
    return ret;
}

bool PushKeyOrButton(const int& keyMode, const DWORD& keyButton)
{
    if (GameDevice()->m_pDI->CheckKey(keyMode, keyButton))
    {
        return true;
    }
    //else if (GameDevice()->m_pDI->)
    return false;
}
