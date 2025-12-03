#include "pch.h"
#include "BIgBullet.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Texture.h"

BIgBullet::BIgBullet()
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"bossBullet");
	auto* com = AddComponent<Collider>();
	com->SetSize({ 50.f,50.f });
	com->SetName(L"BigBullet");
	com->SetTrigger(true);
}

BIgBullet::~BIgBullet()
{
}

void BIgBullet::Update()
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

	Translate({ m_dir.x * 400.f * fDT, m_dir.y * 0.f * fDT });
}

void BIgBullet::Render(HDC _hdc)
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
		0, 0, width, height, RGB(0, 0, 0));
	ComponentRender(_hdc);
}
