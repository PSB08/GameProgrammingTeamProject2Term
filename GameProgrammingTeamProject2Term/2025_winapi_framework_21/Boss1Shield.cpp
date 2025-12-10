#include "pch.h"
#include "Boss1Shield.h"
#include "Boss1.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Animator.h"
#include "Animation.h"

Boss1Shield::Boss1Shield(Boss1* owner,
    const std::wstring& idleTexName,
    const std::wstring& breakTexName,
    Vec2 colliderSize)
    : m_owner(owner)
    , shieldSize(colliderSize.x)
{
    m_idleTexture = GET_SINGLE(ResourceManager)->GetTexture(idleTexName);
    m_breakTexture = GET_SINGLE(ResourceManager)->GetTexture(breakTexName);

    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetName(L"Boss1Shield");
    m_collider->SetSize(colliderSize);

    SetSize(colliderSize);

    m_animator = AddComponent<Animator>();

    if (m_animator && m_idleTexture && m_breakTexture)
        SetupAnimations();
}

Boss1Shield::~Boss1Shield()
{
}

void Boss1Shield::Update()
{
    if (!m_owner || !m_owner->IsActiveBoss())
    {
        SetDead();
        return;
    }

    SetPos(m_owner->GetPos());

    if (!m_animator)
        return;

    if (m_isBreaking)
    {
        Animation* cur = m_animator->GetCurrent();
        if (cur && cur->IsFinished())
        {
            SetDead();
        }
    }
}

void Boss1Shield::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss1Shield::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        GET_SINGLE(ResourceManager)->Play(L"ShieldBlock");
        _other->GetOwner()->SetDead();
    }
}

void Boss1Shield::StartBreak()
{
    if (!m_animator || m_isBreaking)
        return;

    m_isBreaking = true;
    m_animator->Play(m_breakAnimName, PlayMode::Once, 1, 1.f);
}

void Boss1Shield::SetupAnimations()
{
    Vec2 sliceSize = { shieldSize + 100.f , shieldSize + 100.f };
    Vec2 step = { shieldSize + 100.f, 0.f };
    const int frameCount = 10;

    m_idleAnimName = L"boss1Shield_idle";
    m_breakAnimName = L"boss1Shield_break";

    m_animator->CreateAnimation(
        m_idleAnimName,
        m_idleTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.08f
    );

    m_animator->CreateAnimation(
        m_breakAnimName,
        m_breakTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.07f
    );

    m_animator->Play(m_idleAnimName, PlayMode::Loop, 1, 1.f);
}