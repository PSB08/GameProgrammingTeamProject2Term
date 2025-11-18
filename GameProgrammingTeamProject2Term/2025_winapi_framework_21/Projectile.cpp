#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
Projectile::Projectile()
	: // m_dir(-1.f)
	m_angle(0.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"bullet");
	auto* com = AddComponent<Collider>();
	com->SetSize({ 20.f,20.f });
	com->SetName(L"PlayerBullet");
	com->SetTrigger(true);
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	//Translate({ cosf(m_angle)*500.f* fDT,  sinf(m_angle) * 500.f * fDT});
	m_lifeTime += fDT;
	if (m_lifeTime >= 1.5f)
	{
		SetDead();
		return;
	}

	Translate({ m_dir.x * 500.f * fDT, m_dir.y * 500.f * fDT });
}

void Projectile::Render(HDC _hdc)
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
		0, 0, width, height, RGB(255,0,255));
		ComponentRender(_hdc);
}
