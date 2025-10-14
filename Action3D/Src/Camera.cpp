#include "Camera.h"

CCamera::CCamera()
{

}

CCamera::~CCamera()
{
}

void CCamera::Update()
{
    transform.rotation.y += RsticX()* 1.0f * DegToRad;
    MATRIX4X4 mat = XMMatrixRotationY(
    transform.rotation.y);

    VECTOR3 eyePt = VECTOR3(0, 2, -5);
    VECTOR3 lookPt = VECTOR3(0, 1, 0);
    VECTOR3 comPos = transform.position + eyePt* mat;
    VECTOR3 camLook = transform.position + lookPt * mat;  // Look‚Í‚˜‚Æ‚š‚ª‚O‚¾‚©‚ç‰ñ“]i–matj‚µ‚Ä‚à•Ï‚í‚ç‚È‚¢ (1, 2, 0)‚É‚·‚é‚ÆŽ‹“_‚ªŽÎ‚ßŒã‚ë‚É‚È‚é

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
