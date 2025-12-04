#include "pch.h"
#include "BossScene.h"
#include "Object.h"
#include "Player.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Boss1.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "Floor.h"
#include "DeadFloor.h"

void BossScene::Init()
{
	GET_SINGLE(ResourceManager)->Play(L"BOSS1BGM");
	m_pBackground = GET_SINGLE(ResourceManager)->GetTexture(L"BackTest3");

	Object* obj = new Boss1;
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

	Spawn<DeadFloor>
		(Layer::DEFAULT, { WINDOW_WIDTH / 2, 700 }, { 1000, 1000 });

	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSSCORE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSSPROJECTILE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::LASER);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULT);
}

void BossScene::Update()
{
	Scene::Update();
}

void BossScene::Render(HDC hdc)
{
	if (m_pBackground)
	{
		HDC texDC = m_pBackground->GetTextureDC();
		int texW = (int)m_pBackground->GetWidth();
		int texH = (int)m_pBackground->GetHeight();
		::StretchBlt(
			hdc,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			texDC,
			0, 0, texW, texH,
			SRCCOPY
		);
	}

	Scene::Render(hdc);
}
