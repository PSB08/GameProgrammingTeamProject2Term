#include "pch.h"
#include "Boss2MainCore.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Boss1.h"
#include "EventBus.h"

Boss2MainCore::Boss2MainCore(Boss2* boss)
    : m_owner(boss)
    , m_pendingSceneChange(false)
    , m_delay(0.f)
    , m_pTexture(nullptr)
    , m_animator(nullptr)
{
    SetSize({ 100.f, 100.f });

    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2Maincore");

    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"Boss2MainCore");

    m_animator = AddComponent<Animator>();
    if (m_pTexture && m_animator)
        SetupAnimations();
}

Boss2MainCore::~Boss2MainCore()
{
}

void Boss2MainCore::SetupAnimations()
{
    if (!m_pTexture || !m_animator)
        return;

    int texW = m_pTexture->GetWidth();
    int texH = m_pTexture->GetHeight();

    const int frameCount = 7;

    Vec2 sliceSize = { 160.f, 160.f };
    Vec2 step = { 160.f, 0.f };

    m_animator->CreateAnimation(
        L"boss2MainCoreLoop",
        m_pTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.1f
    );

    m_animator->Play(L"boss2MainCoreLoop", PlayMode::Loop, 1, 1.f);
}

void Boss2MainCore::Update()
{
    if (!m_pendingSceneChange)
        return;

    m_delay -= GET_SINGLE(TimeManager)->GetDT();

    if (m_delay <= 0.f)
    {
        GET_SINGLE(SceneManager)->RequestLoadScene(L"BossSelectScene");

        m_pendingSceneChange = false;
    }
}

void Boss2MainCore::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss2MainCore::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        EventBus::Invoke(L"Boss2Killed");

        m_delay = 0.2f;
        m_pendingSceneChange = true;
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
        GET_SINGLE(ResourceManager)->Play(L"BossDie");
    }
}
