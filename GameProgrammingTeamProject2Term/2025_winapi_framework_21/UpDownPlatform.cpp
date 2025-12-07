#include "pch.h"
#include "UpDownPlatform.h"
#include "Collider.h"
#include "ResourceManager.h"

UpDownPlatform::UpDownPlatform()
	: m_DownCheckTimer(5.f),
	m_UpCheckValue(10),
	m_sizeDownValue(100.f)
{
	m_Texture = GET_SINGLE(ResourceManager)->GetTexture(L"Platform");
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

	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_Texture->GetWidth();
	int height = m_Texture->GetHeight();

	::TransparentBlt(_hdc
		, (int)(pos.x - size.x / 2)
		, (int)(pos.y - size.y / 2)
		, size.x, size.y,
		m_Texture->GetTextureDC(),
		0, 0, width, height, RGB(0, 0, 0));
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
