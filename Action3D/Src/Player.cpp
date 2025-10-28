#include "Player.h"

#include "PlayScene.h"

class PlayScene;

CPlayer::CPlayer()
{
    mesh = new CFbxMesh();
    animator = new Animator();
    mesh->Load("Data/Player/PlayerChara.mesh");
    animator->SetModel(mesh);
    mesh->LoadAnimation(0, "Data/Player/Run.anmx", true);

    animator->Play(0, true);
}

CPlayer::~CPlayer()
{
}

void CPlayer::ImGUi(int x, int y, int b)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Pad");
    ImGui::Text("x:%lf", transform.rotation.y * RadToDeg);
    ImGui::End();
}


inline float CalcVector2Angle(const VECTOR3& vNorm1, const VECTOR3& vNorm2)
{
    float angle = acosf(vNorm1.x * vNorm2.x + vNorm1.y * vNorm2.y + vNorm1.z * vNorm2.z);
    return angle * RadToDeg;
}

void CPlayer::Update()
{
    Object3D::Update();
    auto inp = GameDevice()->m_pDI->GetJoyState();
    int x = inp.lRx;
    int y = inp.lRy;
    int b = inp.rgbButtons[0];
    ImGUi(x, y, b);
    CDirectInput* di = GameDevice()->m_pDI;
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
        }else if (diff < 0)
        {
            transform.rotation.y -= 30.0f * DegToRad;
        }
#else
#endif
        transform.position += (velocity * 0.5f);
    }
    XMMATRIX mat = XMMatrixRotationY(transform.rotation.y);
    VECTOR3 front = VECTOR3(0, 0, 1) * mat;
    VECTOR3 right = VECTOR3(1, 0, 0) * mat;
    VECTOR3 up = VECTOR3(0, 1, 0) * mat;
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
