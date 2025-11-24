#include "pch.h"
#include "TestBoss2Scene.h"
#include "Object.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Boss2.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "Floor.h"

void TestBoss2Scene::Init()
{
	Object* obj = new Boss2;
	obj->SetPos({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });
	obj->SetSize({ 150,150 });

	AddObject(obj, Layer::BOSS);

	Spawn<Player>
		(
			Layer::PLAYER
			, { WINDOW_WIDTH / 2, 550 }
	, { 100,100 });

	Spawn<Floor>
		(
			Layer::DEFAULT
			, { WINDOW_WIDTH / 2, 600 }
	, { 100,100 });

	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSSCORE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSSPROJECTILE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::LASER);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULT);
}

void TestBoss2Scene::Update()
{
	Scene::Update();
}
