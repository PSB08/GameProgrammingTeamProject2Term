#include "pch.h"
#include "ExploseProjectile.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Rigidbody.h"

ExploseProjectile::ExploseProjectile()
	: m_angle(0.f),
	  m_maxLifeTime(3.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"plane");  //¹Ù²ã¾ßÇÔ
	auto* com = AddComponent<Collider>();
	AddComponent<Rigidbody>();
	com->SetSize({ 20.f,20.f });
	com->SetName(L"BossProjectile");
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
	for (int i = 0; i < _value; i++)
	{

	}
}


void ExploseProjectile::EnterCollision(Collider* _other)
{
	Rigidbody* rig = GetComponent<Rigidbody>();
	if(_other->GetName() == L"Floor")
	{
		SetDead();
		
		return;
	}
}
