#include "pch.h"
#include "Boss1Core.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Boss1.h"
#include "EventBus.h"
#include "Effect.h"

Boss1Core::Boss1Core(Boss1* owner)
    : m_owner(owner)
    , m_pTexture(nullptr)
    , m_animator(nullptr)
{
    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss1Core");

    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"BossCore");
    col->SetSize({ 150.f, 150.f });

    SetSize({ 1.f, 1.f });

    m_animator = AddComponent<Animator>();
    if (m_pTexture && m_animator)
        SetupAnimations();
}

Boss1Core::~Boss1Core()
{
}

void Boss1Core::Update()
{
    if (!m_pendingSceneChange)
        return;

    m_delay -= GET_SINGLE(TimeManager)->GetDT();

    if (m_delay <= 0.f)
    {
        GET_SINGLE(SceneManager)->RequestLoadScene(L"ClearScene");

        m_pendingSceneChange = false;
    }
}

void Boss1Core::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss1Core::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        EventBus::Invoke(L"Boss1Killed");

        auto* effect = new Effect;

        effect->SetPos(GetPos());
        effect->SetSize({ 160.f, 160.f });

        Texture* tex = GET_SINGLE(ResourceManager)->GetTexture(L"BigExplosion");
        effect->Init(
            tex,
            L"explosionOnce",
            { 0.f, 0.f }, 
            { 160.f, 160.f },
            { 160.f, 0.f },
            11,
            0.06f
        );

        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(effect, Layer::BUTTON);

        m_delay = 1.f;
        m_pendingSceneChange = true;
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
        GET_SINGLE(ResourceManager)->Play(L"BossDie");
    }
}

void Boss1Core::SetupAnimations()
{
    int texW = m_pTexture->GetWidth();
    int texH = m_pTexture->GetHeight();

    const int frameCount = 10;

    Vec2 sliceSize = { 160.f, 160.f };
    Vec2 step = { 160.f, 0.f };

    m_animator->CreateAnimation(
        L"coreLoop",
        m_pTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.1f
    );

    m_animator->Play(L"coreLoop", PlayMode::Loop, 1, 1.f);
}