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
#include "DeadFloor.h"
#include "UpDownPlatform.h"
#include "Button.h"

void Boss3Scene::Init()
{
	GET_SINGLE(ResourceManager)->Play(L"BOSS3BGM");
	m_pBackground = GET_SINGLE(ResourceManager)->GetTexture(L"Backtest6");

	Object* obj = new Boss3;
	obj->SetPos({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5 });
	obj->SetSize({ 150,300 });

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

	Spawn<UpDownPlatform>
		(
			Layer::DEFAULT
			, { 320, 450 }
	, { 100,100 });

	Spawn<UpDownPlatform>
		(
			Layer::DEFAULT
			, { 960, 450 }
	, { 100,100 });

	Spawn<Floor>
		(
			Layer::DEFAULT
			, { WINDOW_WIDTH / 2, 600 }
	, { 100,100 });

	Spawn<DeadFloor>
		(Layer::DEFAULT, { WINDOW_WIDTH / 2, 700 }, { 1000, 1000 });

#pragma region  콜라이더 충돌처리 하는 부분
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::BOSSCORE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSS);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BOSSPROJECTILE);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PROJECTILE, Layer::DEFAULT);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::DEFAULT);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::BOSSPROJECTILE, Layer::DEFAULT);
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER, Layer::BUTTON);
#pragma endregion


}

void Boss3Scene::Update()
{
	Scene::Update();
}

void Boss3Scene::Render(HDC hdc)
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
