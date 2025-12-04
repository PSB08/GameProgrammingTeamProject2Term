#include "pch.h"
#include "Boss2Core.h"
#include "Boss2.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Animator.h"
#include "Animation.h"
#include "TimeManager.h"

Boss2Core::Boss2Core(Boss2* owner, int index)
    : m_owner(owner)
    , m_index(index)
    , m_isOpened(false)
    , m_collidable(false)
    , m_hp(1)
    , m_maxHp(1)
    , m_collider(nullptr)
    , m_animator(nullptr)
    , m_state(CoreState::Idle)
    , m_openTimer(0.f)
    , m_openDuration(5.f)
{
    SetSize({ 60.f, 60.f });

    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2Subcore");
    m_pDeathTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2SubcoreDeath");
    m_pBrokenTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2SubcoreBreak");
    m_pBrokingTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2SubcoreBreaking");
    m_pReturnTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2SubcoreReturn");

    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetSize(GetSize());
    m_collider->SetName(L"Boss2Core");
    m_collider->SetEnabled(false);

    m_animator = AddComponent<Animator>();
    if (m_animator)
        SetupAnimations();
}

Boss2Core::~Boss2Core()
{
}

void Boss2Core::SetupAnimations()
{
    if (!m_animator)
        return;

    const float frameW = 160.f;
    const int   frameCount = 8;

    //일반/닫힌 상태 + 돌아가는 애니메이션
    if (m_pTexture)
    {
        int texH = m_pTexture->GetHeight();

        Vec2 slice = { frameW, (float)texH };
        Vec2 step = { frameW, 0.f };

        // 닫힌 Idle 상태
        m_animator->CreateAnimation(
            L"CoreIdle",
            m_pTexture,
            { 0.f, 0.f },
            slice,
            step,
            frameCount,
            0.1f
        );

        // 열렸다가 되돌아갈 때 애니메이션
        m_animator->CreateAnimation(
            L"CoreReturn",
            m_pReturnTexture,
            { 0.f, 0.f },
            slice,
            step,
            frameCount + 1,
            0.06f
        );
    }

    // 열려 있을 때 + 파괴 시 애니메이션
    if (m_pBrokenTexture)
    {
        int texH = m_pBrokenTexture->GetHeight();

        Vec2 slice = { frameW, (float)texH };
        Vec2 step = { frameW, 0.f };

        // 코어가 열린 상태 애니메이션
        m_animator->CreateAnimation(
            L"CoreOpened",
            m_pBrokenTexture,
            { 0.f, 0.f },
            slice,
            step,
            frameCount - 1,
            0.08f
        );

        // 총알 맞고 터질 때 애니메이션
        m_animator->CreateAnimation(
            L"CoreBreaking",
            m_pBrokingTexture,
            { 0.f, 0.f },
            slice,
            step,
            frameCount + 1,
            0.05f
        );
    }

    if (m_pDeathTexture)
    {
        int texH = m_pDeathTexture->GetHeight();

        Vec2 slice = { frameW, (float)texH };
        Vec2 step = { frameW, 0.f };

        m_animator->CreateAnimation(
            L"Death",
            m_pDeathTexture,
            { 0.f, 0.f },
            slice,
            step,
            10,
            0.08f
        );
    }

    ResetToIdle();
}

void Boss2Core::ResetToIdle()
{
    m_state = CoreState::Idle;
    m_isOpened = false;
    m_collidable = false;
    m_hp = m_maxHp;
    m_openTimer = 0.f;

    if (m_collider)
        m_collider->SetEnabled(false);

    if (m_animator)
        m_animator->Play(L"CoreIdle", PlayMode::Loop, 1, 1.f);
}

void Boss2Core::Update()
{
    float dt = GET_SINGLE(TimeManager)->GetDT();

    switch (m_state)
    {
    case CoreState::Idle:
        // 닫혀있는 상태, 아무것도 안 함
        break;

    case CoreState::Opening:
        //열리는 애니메이션이 끝났는지 체크
        if (m_animator)
        {
            Animation* cur = m_animator->GetCurrent();
            if (cur && cur->IsFinished())
            {
                // 이제 진짜 열린 상태
                m_state = CoreState::Opened;
                m_collidable = true;
                m_openTimer = 0.f;

                if (m_collider)
                    m_collider->SetEnabled(true);
            }
        }
        break;

    case CoreState::Opened:
        // 열려 있는 동안 5초 카운트
        m_openTimer += dt;
        if (m_openTimer >= m_openDuration)
        {
            // 3초 동안 안 부서졌으니 되돌아가기 시작
            m_state = CoreState::Returning;
            m_isOpened = false;
            m_collidable = false;
            if (m_collider)
                m_collider->SetEnabled(false);

            if (m_animator)
                m_animator->Play(L"CoreReturn", PlayMode::Once, 1, 1.f);
        }
        break;

    case CoreState::Returning:
        if (m_animator)
        {
            Animation* cur = m_animator->GetCurrent();
            if (cur && cur->IsFinished())
            {
                // 돌아오는 애니메이션 끝 => 완전 닫힌 상태로 복귀
                ResetToIdle();
            }
        }
        break;

    case CoreState::Breaking:
        if (m_animator)
        {
            Animation* cur = m_animator->GetCurrent();
            if (cur && cur->IsFinished())
            {
                // 완전히 부서짐 => 이 코어는 게임에서 제거
                SetDead();
            }
        }
        break;
    }

    //열린 상태에서만 체크하고, 파괴되면 즉시 HandleDeath()
    if (m_state == CoreState::Opened && m_isOpened && m_hp <= 0)
    {
        HandleDeath();
    }
}

void Boss2Core::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss2Core::EnterCollision(Collider* _other)
{
    // 닫혀 있거나, 열려 있어도 충돌 비활성이면 무시
    if (m_state != CoreState::Opened)
        return;

    if (!m_collidable)
        return;

    if (_other->GetName() == L"PlayerBullet")
    {
        m_hp -= 100;
        _other->GetOwner()->SetDead();
    }
}

void Boss2Core::OpenCore()
{
    // 이미 죽는 중이면 무시
    if (m_state == CoreState::Breaking)
        return;

    // 보스가 코어를 열었을 때 호출됨
    m_state = CoreState::Opening;
    m_isOpened = true;
    m_collidable = false;
    m_openTimer = 0.f;
    m_hp = m_maxHp;

    if (m_collider)
        m_collider->SetEnabled(false);

    // 열리는 애니메이션 1회 재생
    if (m_animator)
        m_animator->Play(L"CoreBreaking", PlayMode::Once, 1, 1.f);
}

void Boss2Core::HandleDeath()
{
    // 이미 죽는 중이면 무시
    if (m_state == CoreState::Breaking)
        return;

    // 이 지점이 클리어 조건용 파괴
    m_state = CoreState::Breaking;
    m_isOpened = false;
    m_collidable = false;
    if (m_collider)
        m_collider->SetEnabled(false);

    if (m_owner)
        m_owner->NotifyCoreDestroyed(this); // Boss 패턴 로직용

    if (m_animator)
        m_animator->Play(L"Death", PlayMode::Once, 1, 1.f);
}
