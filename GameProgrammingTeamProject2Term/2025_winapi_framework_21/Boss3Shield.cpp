#include "pch.h"
#include "Boss3Shield.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "ResourceManager.h"

Boss3Shield::Boss3Shield(Object* owner,
    const std::wstring& idleTexName, const std::wstring& breakTexName,
    Vec2 shieldSize)
    : m_owner(owner),
    m_shieldSize(shieldSize.x)

{
    m_idleTexture = GET_SINGLE(ResourceManager)->GetTexture(idleTexName);
    m_breakTexture = GET_SINGLE(ResourceManager)->GetTexture(breakTexName);

    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetName(L"Boss3Shield");
    m_collider->SetSize(shieldSize);

    SetSize(shieldSize);

    m_animator = AddComponent<Animator>();

    if (m_animator && m_idleTexture && m_breakTexture)
        SetupAnimations();
}

Boss3Shield::~Boss3Shield()
{
}

void Boss3Shield::Update()
{
    if (!m_owner || m_owner->GetIsDead())
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

void Boss3Shield::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss3Shield::EnterCollision(Collider* other)
{
    if (other->IsTrigger() && other->GetName() == L"PlayerBullet")
    {
        other->GetOwner()->SetDead();
    }
}

void Boss3Shield::StartBreak()
{
    if (!m_animator || m_isBreaking)
        return;

    m_isBreaking = true;
    m_isBroken = true;

    m_animator->Play(m_breakAnimName, PlayMode::Once, 1, 1.f);
}

void Boss3Shield::SetupAnimations()
{
    Vec2 sliceSize = { m_shieldSize + 100.f , m_shieldSize + 100.f };
    Vec2 step = { m_shieldSize + 100.f , 0.f };

    const int frameCount = 10;

    m_idleAnimName = L"boss3Shield_idle_" + std::to_wstring((uintptr_t)this);
    m_breakAnimName = L"boss3Shield_break_" + std::to_wstring((uintptr_t)this);

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
        0.06f
    );

    m_animator->Play(m_idleAnimName, PlayMode::Loop, 1, 1.f);
}
