#include "pch.h"
#include "Explosion.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Animator.h"
#include "ResourceManager.h"

Explosion::Explosion()
	: m_explosingTime(.64f)
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 485.f, 485.f });
	col->SetName(L"Explosion");
    GET_SINGLE(SceneManager)->GetCurScene()->StartShake(0.6f, 150.f);
    auto* ani = AddComponent<Animator>();

    m_ExpTexture = GET_SINGLE(ResourceManager)->GetTexture(L"VerybigExplosion");
    GET_SINGLE(ResourceManager)->Play(L"Explosion");
    ani->CreateAnimation
    (L"Explose"
        , m_ExpTexture
        , { 0.f,0.f }
        , { 720.f,720.f }
        , { 720.f,0.f }
    , 11, 0.05f);

    ani->Play(L"Explose", PlayMode::Once, 1);
}

Explosion::~Explosion()
{
}

void Explosion::Update()
{
    if (m_timer < m_explosingTime)
        m_timer += fDT;
    else
        SetDead();
}

void Explosion::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}
