#include "pch.h"
#include "BossProjectile.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"

BossProjectile::BossProjectile()
	: m_angle(0.f)
	, m_dir(0.f, 0.f)
	, m_launched(false)
	, m_speed(500.f)
	, m_lifeTime(0.f)
	, m_collider(nullptr)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"bossBullet");
	auto* com = AddComponent<Collider>();
	com->SetSize({ 20.f,20.f });
	com->SetName(L"BossProjectile");
	com->SetTrigger(true);
}

BossProjectile::~BossProjectile()
{
}

void BossProjectile::Update()
{
	if (!m_launched)
	{
		return;
	}

	m_lifeTime += fDT;
	if (m_lifeTime >= 6.f)
	{
		SetDead();
		return;
	}

	Translate({ m_dir.x * 500.f * fDT, m_dir.y * 500.f * fDT });
}

void BossProjectile::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();

	::TransparentBlt(_hdc
		, (int)(pos.x - size.x / 2)
		, (int)(pos.y - size.y / 2)
		, size.x, size.y,
		m_pTexture->GetTextureDC(),
		0, 0, width, height, RGB(255, 0, 255));
	ComponentRender(_hdc);
}

void BossProjectile::SetColliderName(const std::wstring& name)
{
	if (m_collider)
		m_collider->SetName(name);
}