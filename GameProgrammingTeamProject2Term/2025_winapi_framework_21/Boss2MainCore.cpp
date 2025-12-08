#include "pch.h"
#include "Boss2MainCore.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Boss1.h"
#include "EventBus.h"
#include "Effect.h"
#include "Boss2Shield.h"

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
    InitShields();
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
        GET_SINGLE(SceneManager)->RequestLoadScene(L"ClearScene");

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
        if (HasShield())
        {
            _other->GetOwner()->SetDead();
            return;
        }

        EventBus::Invoke(L"Boss2Killed");

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

void Boss2MainCore::InitShields()
{
    float sizes[4] = { 150.f, 250.f, 300.f, 350.f };

    std::wstring idleNames[4] =
    {
        L"ShieldIdle150",
        L"ShieldIdle250",
        L"ShieldIdle300",
        L"ShieldIdle350"
    };

    std::wstring breakNames[4] =
    {
        L"Shield150",
        L"Shield250",
        L"Shield300",
        L"Shield350"
    };

    Vec2 corePos = GetPos();
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    //따로따로 할 예정
    for (int i = 0; i < 4; ++i)
    {
        float s = sizes[i];
        Boss2Shield* shield = new Boss2Shield(
            this,
            idleNames[i],
            breakNames[i],
            { s - 100.f, s - 100.f }
        );

        shield->SetPos(corePos);
        scene->AddObject(shield, Layer::BOSSCORE);

        m_shields[i] = shield;
    }

    m_shieldCount = 4;
}

void Boss2MainCore::BreakNextShield()
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_shields[i] &&
            !m_shields[i]->IsBreaking() &&
            !m_shields[i]->GetIsDead())
        {
            m_shields[i]->StartBreak();
            --m_shieldCount;
            break;
        }
    }
}

void Boss2MainCore::ResetShields()
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_shields[i])
        {
            m_shields[i]->SetDead();
            m_shields[i] = nullptr;
        }
    }

    InitShields();
}