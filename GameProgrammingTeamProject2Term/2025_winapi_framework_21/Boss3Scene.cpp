#include "pch.h"
#include "Boss3Scene.h"
#include "Object.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Boss3.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "Floor.h"
#include "UpDownPlatform.h"

void Boss3Scene::Init()
{
	Object* obj = new Boss3;
	obj->SetPos({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });
	obj->SetSize({ 150,150 });

	AddObject(obj, Layer::BOSS);

	Spawn<Player>
		(
			Layer::PLAYER
			, { WINDOW_WIDTH / 2, 550 }
	, { 100,100 });

	Spawn<UpDownPlatform>
		(
			Layer::DEFAULT
			, { WINDOW_WIDTH / 2, 450 }
	, { 100,100 });

	Spawn<Floor>
		(
			Layer::DEFAULT
			, { WINDOW_WIDTH / 2, 600 }
	, { 100,100 });

#pragma region  콜라이더 충돌처리 하는 부분
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSSCORE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSSPROJECTILE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::DEFAULT);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULT);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::BOSSPROJECTILE, Layer::DEFAULT);
#pragma endregion


}

void Boss3Scene::Update()
{
	Scene::Update();
}
