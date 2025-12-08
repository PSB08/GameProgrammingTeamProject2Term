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
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"BulletG");

	auto* com = AddComponent<Collider>();
	Vec2 size = GetSize();
	com->SetSize(size);
	com->SetName(L"ExploseProjectile");
	com->SetTrigger(true);

	AddComponent<Rigidbody>();

}

ExploseProjectile::~ExploseProjectile()
{
}

void ExploseProjectile::Update()
{
	if (m_useRigid == false)
	{
		auto* rigid = GetComponent<Rigidbody>();
		rigid->SetUseGravity(false);
		Translate({ m_dir * fDT * 400.f});
	}
	m_lifeTime += fDT;
	m_delayTime += fDT;
	if (m_lifeTime >= m_maxLifeTime)
	{
		SetDead();
		return;
	}
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
}

void ExploseProjectile::Explose(int _value)
{
	Vec2 center = GetPos();

	if (m_gravityExplosion == true)
	{
		if (m_endDivision == true)
		{
			Rigidbody* _rb = GetComponent<Rigidbody>();
			float dx = rand() + 50.f;
			for (int i = 0; i < 2; i++)
			{
				dx *= -1;
				Vec2 dir = { dx, -350.f };

				auto* proj = new ExploseProjectile;
				proj->SetPos({ center.x, center.y });
				proj->SetSize({ 10.f, 10.f });
				proj->SetDivision(false);
				proj->SetGravity(true);
				proj->SetForce(dir);
				GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
			}
		}
	}
	else
	{
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
}

void ExploseProjectile::SetGravity(bool _value)
{
	m_gravityExplosion = _value;
}

void ExploseProjectile::SetDivision(bool _value)
{
	m_endDivision = _value;

	if (!_value)
		m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"BulletP");
}

void ExploseProjectile::SetRigid(bool _value)
{
	m_useRigid = _value;
}

void ExploseProjectile::SetValue(int _value)
{
	m_ExploseValue = _value;
}

void ExploseProjectile::SetForce(Vec2 _force)
{
	auto* rigid = GetComponent<Rigidbody>();
	rigid->AddImpulse(_force);
}


void ExploseProjectile::EnterCollision(Collider* _other)
{
	if((_other->GetName() == L"Floor" || _other->GetName() == L"Player" ||
		_other->GetName() == L"Platform") && m_delayTime > 0.1f)
	{
		Explose(m_ExploseValue);
		SetDead();
		return;
	}
}
