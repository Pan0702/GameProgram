#include "PlayScene.h"
#include <fstream>
#include "../CsvReader.h"
#include <assert.h>

#include "../Player/Camera.h"
#include "../Field.h"
#include "../GameObject.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Golem.h"
#include "../Player/Player.h"

PlayScene::PlayScene()
{
	Instantiate<CPlayer>();
	Instantiate<CField>();
	Instantiate<CCamera>();
	for (int i = 10; --i >= 0;)
	{
		new CGolem(VECTOR3(1.5 * i, 0, 10));
	}
	Instantiate<CEnemyManager>();
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
