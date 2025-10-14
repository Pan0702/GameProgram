#include "PlayScene.h"
#include <fstream>
#include "CsvReader.h"
#include <assert.h>

#include "Camera.h"
#include "Field.h"
#include "GameObject.h"
#include "Player.h"

PlayScene::PlayScene()
{
	Instantiate<CPlayer>();
	Instantiate<CField>();
	Instantiate<CCamera>();
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
