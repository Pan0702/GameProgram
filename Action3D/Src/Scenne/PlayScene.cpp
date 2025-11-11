#include "PlayScene.h"
#include <fstream>
#include "../CsvReader.h"
#include <assert.h>

#include "../Player/Camera.h"
#include "../Field.h"
#include "../GameObject.h"
#include "../Enemy/Golem.h"
#include "../Player/Player.h"

PlayScene::PlayScene()
{
	Instantiate<CPlayer>();
	Instantiate<CField>();
	Instantiate<CCamera>();
		new CGolem(VECTOR3(5, 0, 10));
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "PlayScene", 16, RGB(255, 255, 0));
}
