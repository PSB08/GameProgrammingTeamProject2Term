#include "pch.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "SceneManager.h"
#include "FollowProjectile.h"
#include "Transform.h"
#include "Player.h"
#include "Enums.h"

FollowProjectile::FollowProjectile()
	: m_angle(0.f),
	m_waitTimerValue(1.f),
	m_exploseTimerValue(1.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"plane");  //바꿔야함
	auto* com = AddComponent<Collider>();
	com->SetSize({ 30.f,30.f });
	com->SetName(L"ExploseProjectile");
	com->SetTrigger(true);
	GetParentPosition();
}

FollowProjectile::~FollowProjectile()
{
}

void FollowProjectile::Update()
{
	if (!m_exploseState)
	{
		if (m_timer < m_waitTimerValue)
		{
			m_timer += fDT;
			GetPlayerPosition();
		}
		else
		{
			Translate({-(m_myPos.x - m_playerPos.x) * 2.f * fDT, -(m_myPos.y - m_playerPos.y) * 2.f * fDT }); // 위치벡터 구하기
		}
	}
	else
	{
		if (m_exploseTimer < m_exploseTimerValue)
		{
			m_exploseTimer += fDT;
			Explose();
		}
	}

}

void FollowProjectile::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();
	/*ELLIPSE_RENDER(_hdc, pos.x, pos.y
		, size.x, size.y);*/
	::TransparentBlt(_hdc
		, (int)(pos.x - size.x / 2)
		, (int)(pos.y - size.y / 2)
		, size.x, size.y,
		m_pTexture->GetTextureDC(),
		0, 0, width, height, RGB(255, 0, 255));
	ComponentRender(_hdc);
}

void FollowProjectile::Explose()
{
	cout << "Boom";
	SetDead();
}

void FollowProjectile::GetParentPosition()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		const auto& objects = GET_SINGLE(SceneManager)->GetCurScene()->GetLayerObjects((Layer)i);
		for (Object* obj : objects)
		{
			if (!obj)
				continue;
			if((Layer)i == Layer::BOSS)
				m_myPos = obj->GetPos();
		}
	}
}

void FollowProjectile::GetPlayerPosition()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		const auto& objects = GET_SINGLE(SceneManager)->GetCurScene()->GetLayerObjects((Layer)i);
		for (Object* obj : objects)
		{
			if (!obj)
				continue;
			if ((Layer)i == Layer::PLAYER)
				m_playerPos = obj->GetPos();
		}
	}
}


void FollowProjectile::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor")
	{
		m_exploseState = true;
		m_exploseTimer = true;
		m_playerPos = {};
	}
}