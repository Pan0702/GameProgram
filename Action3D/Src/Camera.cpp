#include "Camera.h"

#include "Player.h"

namespace
{
    const auto EYE_BASE = VECTOR3(0, 0, -5.2f);
    constexpr float ANGLE_BASE = 30.0f * DegToRad;
}

CCamera::CCamera()
{
    transform.rotation.x = ANGLE_BASE;
}

CCamera::~CCamera()
{
}

float ClampMe(float& d, const float& minV, const float& maxV)
{
    if (d < minV) d = minV;
    if (d > maxV) d = maxV;
    return d;
}

void CCamera::Update()
{
    //transform.position = ObjectManager::FindGameObject<CPlayer>()->GetPlayerPos();
    transform.rotation.y += RsticX() * 1.0f * DegToRad;
    transform.rotation.x += RsticY() * 1.0f * DegToRad;
    ClampMe(transform.rotation.x, -30.0f * DegToRad, 85.0f * DegToRad);
    ImGui::Begin("Camera");
    ImGui::Text("%lf", transform.rotation.y * RadToDeg);
    ImGui::End();
    MATRIX4X4 matY = XMMatrixRotationY(
        transform.rotation.y);
    MATRIX4X4 matX = XMMatrixRotationX(
        transform.rotation.x);
    MATRIX4X4 mat = matX * matY;

    VECTOR3 eyePt = EYE_BASE * mat + VECTOR3(0, 1.5, 0);
    auto lookPt = VECTOR3(0, 1, 0);
    GameDevice()->m_vEyePt = eyePt;
    GameDevice()->m_vLookatPt = lookPt;
    VECTOR3 comPos = transform.position + eyePt;
    VECTOR3 camLook = transform.position + lookPt * mat; // Look‚Í‚˜‚Æ‚š‚ª‚O‚¾‚©‚ç‰ñ“]i–matj‚µ‚Ä‚à•Ï‚í‚ç‚È‚¢ (1, 2, 0)‚É‚·‚é‚ÆŽ‹“_‚ªŽÎ‚ßŒã‚ë‚É‚È‚é

    GameDevice()->m_mView = XMMatrixLookAtLH(
        comPos, camLook, VECTOR3(0, 1, 0));
}

float CCamera::RsticX()
{
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_RIGHT)) return 1.0f;
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_LEFT)) return -1.0f;
    if (GameDevice()->m_pDI->GetJoyState().lRx == 0) return 0;
    auto inp = GameDevice()->m_pDI->GetJoyState();
    float x = inp.lRx;

    x = x / 65535.0f * 2.0f - 1.0f;

    return x;
}

float CCamera::RsticY()
{
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_DOWN)) return 1.0f;
    if (GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_UP)) return -1.0f;
    if (GameDevice()->m_pDI->GetJoyState().lRy == 0) return 0;
    auto inp = GameDevice()->m_pDI->GetJoyState();
    float x = inp.lRy;

    x = x / 65535.0f * 2.0f - 1.0f;

    return x;
}
