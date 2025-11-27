#include "pch.h"
#include "UpDownPlatform.h"
#include "Collider.h"

UpDownPlatform::UpDownPlatform()
	: m_DownCheckTimer(5.f),
	m_UpCheckValue(10),
	m_sizeDownValue(100.f)
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 100.f, 50.f });
	col->SetName(L"Platform");
}

UpDownPlatform::~UpDownPlatform()
{

}

void UpDownPlatform::Update()
{
	Vec2 platformPosition = GetPos();
	float platformY = platformPosition.y;
	if (m_nowSize > 1 && m_timer < m_DownCheckTimer)
	{
		m_timer += fDT;
	}

	if (m_timer > m_DownCheckTimer)
	{
		m_timer = 0.f;	
		m_nowSize--;

		platformY += 100;

		SetPos({ platformPosition.x, platformY});
	}

	if (m_hitValue >= m_UpCheckValue && m_nowSize < 4)
	{
		m_hitValue = 0;
		m_nowSize++;
		
		platformY -= 100;

		SetPos({platformPosition.x , platformY});
	}
}

void UpDownPlatform::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}

void UpDownPlatform::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"PlayerBullet")
	{
		_other->GetOwner()->SetDead();
		m_timer = 0.f;
		cout << m_hitValue;
		m_hitValue++;
	}
}
