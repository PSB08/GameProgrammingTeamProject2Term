#include "pch.h"
#include "DevScene.h"
#include "Object.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "Floor.h"

void DevScene::Init()
{
	// Object 배치
	Object* obj = new Enemy;
	obj->SetPos({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });
	obj->SetSize({ 100,100 });
	//obj->SetScene(this);
	AddObject(obj, Layer::ENEMY);

	Spawn<Player>
		(
			Layer::PLAYER
			, { WINDOW_WIDTH / 2, 300}
			, { 100,100 });

	Spawn<Floor>
		(
			Layer::DEFAULT
			, { WINDOW_WIDTH / 2, 600 }
	, { 100,100 });
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::ENEMY);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::ENEMY);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULT);
	//GET_SINGLE(ResourceManager)->Play(L"BGM");
}

void DevScene::Update()
{
	Scene::Update();
	// 엔터가 눌리면 씬을 변경
	if (GET_KEY(KEY_TYPE::ENTER))
		GET_SINGLE(SceneManager)->LoadScene(L"BossScene");
}

