#include "pch.h"
#include "ExploseProjectile.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "SceneManager.h"
#include "BossProjectile.h"

ExploseProjectile::ExploseProjectile()
	: m_angle(0.f),
	  m_maxLifeTime(3.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"plane");  //¹Ù²ã¾ßÇÔ
	auto* com = AddComponent<Collider>();
	AddComponent<Rigidbody>();
	com->SetSize({ 30.f,30.f });
	com->SetName(L"ExploseProjectile");
	com->SetTrigger(true);
}

ExploseProjectile::~ExploseProjectile()
{
}

void ExploseProjectile::Update()
{

	m_lifeTime += fDT;
	if (m_lifeTime >= m_maxLifeTime)
	{
		SetDead();
		return;
	}

	Translate({ m_dir.x * 500.f * fDT, m_dir.y * 500.f * fDT });
}

void ExploseProjectile::Render(HDC _hdc)
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

void ExploseProjectile::Explose(int _value)
{
	Vec2 center = GetPos();

	for (int i = 0; i < _value; i++)
	{
		float ang = (m_angle * PI / 180.f) + (PI * 2.f) * ((float)i / _value);
		Vec2 dir = { cosf(ang), sinf(ang) };

		auto* proj = new BossProjectile;
		proj->SetPos(center);
		proj->SetSize({ 20.f, 20.f });
		proj->SetDir(dir);

		GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
	}
}


void ExploseProjectile::EnterCollision(Collider* _other)
{
	if(_other->GetName() == L"Floor" || _other->GetName() == L"Player")
	{
		Explose(12);
		SetDead();
		return;
	}
}
