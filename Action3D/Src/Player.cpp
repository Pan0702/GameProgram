#include "Player.h"

#include "PlayScene.h"

class PlayScene;

CPlayer::CPlayer()
{
    mesh = new CFbxMesh();
    animator = new Animator();
    mesh->Load("Data/Player/PlayerChara.mesh");
    animator->SetModel(mesh);
    mesh->LoadAnimation(0,"Data/Player/Run.anmx",true);

    animator->Play(0, true);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
    Object3D::Update();
    CDirectInput* di = GameDevice()->m_pDI;
    if (di->CheckKey(KD_DAT, DIK_D))
    {
        transform.rotation.y += 3.0f * DegToRad;
    }
    if (di->CheckKey(KD_DAT, DIK_A))
    {
        transform.rotation.y -= 3.0f * DegToRad;
    }
    if (di->CheckKey(KD_DAT, DIK_W))
    {
        // キャラクターの前に進む
        // float speed = 0.05f;
        VECTOR3 move = VECTOR3(0,0,0.05f);  // 回ってない時の移動ベクトル (0,0,1)*0.05fでもおｋ
        //VECTOR3 move = VECTOR3(0,0,1)
        MATRIX4X4 mat = XMMatrixRotationY(
            transform.rotation.y);  // 回転行列

        transform.position =      // 新しい場所 
        transform.position/*元の場所*/ + move * mat/*移動量*/;
        //移動量は回っていない時の移動ベクトル × 回転行列
    }
	
    MATRIX4X4 mat = XMMatrixRotationY(
        transform.rotation.y);

    VECTOR3 comPos = transform.position + VECTOR3(1, 2, -5) * mat;
    VECTOR3 camLook = transform.position + VECTOR3 (0, 1, 0) * mat;  // Lookはｘとｚが０だから回転（＊mat）しても変わらない (1, 2, 0)にすると視点が斜め後ろになる

    GameDevice()->m_mView = XMMatrixLookAtLH(
        comPos, camLook, VECTOR3(0, 1, 0));
	
}

void CPlayer::Draw()
{
    Object3D::Draw();
}
