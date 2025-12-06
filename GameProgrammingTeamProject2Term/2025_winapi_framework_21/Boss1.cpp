#include "pch.h"
#include "Boss1.h"
#include "Boss1Core.h"
#include "BossProjectile.h"
#include "LaserObject.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Animator.h"
#include "Animation.h"
#include "Texture.h"

Boss1::Boss1()
    : m_isCorePhase(false)
    , m_startDelayTimer(0.f)
    , m_angle1(0.f)
    , m_fireTimer1(0.f)
    , m_angle2(0.f)
    , m_fireTimer2(0.f)
    , m_laserLeftX(0.f)
    , m_laserRightX(WINDOW_WIDTH)
    , m_laserActive(false)
    , m_animator(nullptr)
    , m_currLine(0)
    , m_totalLines(4)
    , m_isDying(false)
    , m_pDeathTexture(nullptr)
{
    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss1");
    m_pDeathTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss1CoreBreaking");
    m_animator = AddComponent<Animator>();

    m_deathAnimName = L"boss1_death";

    if (m_pTexture && m_animator)
    {
        SetupAnimations();
        PlayNextLine();
    }
    StartRandomPattern();
}

Boss1::~Boss1()
{
}

void Boss1::StartRandomPattern()
{
    int r = rand() % 3;
    switch (r)
    {
    case 0: m_curPattern = Boss1Pattern::PATTERN1; break;
    case 1: m_curPattern = Boss1Pattern::PATTERN2; break;
    case 2: m_curPattern = Boss1Pattern::PATTERN3; break;
    }

    m_patternTimer = 0.f;
}

void Boss1::UpdatePattern()
{
    if (m_isCorePhase)
        return;

    if (m_startDelayTimer < m_startDelay)
    {
        m_startDelayTimer += fDT;
        return;
    }

    m_patternTimer += fDT;

    switch (m_curPattern)
    {
    case Boss1Pattern::PATTERN1:
        Pattern1();
        if (m_patternTimer > 5.f)
        {
            EndPattern();
        }
        break;

    case Boss1Pattern::PATTERN2:
        Pattern2();
        if (m_patternTimer > 5.f)
        {
            EndPattern();
        }
        break;

    case Boss1Pattern::PATTERN3:
        Pattern3();
        if (m_patternTimer > 3.f)
        {
            m_laserActive = false;
            EndPattern();
        }
        break;

    case Boss1Pattern::NONE:
        break;
    }
}

void Boss1::UpdateRender()
{
    if (m_isDying)
    {
        if (m_animator)
        {
            Animation* curAnim = m_animator->GetCurrent();
            if (curAnim && curAnim->IsFinished())
            {
                SetActiveBoss(false);
                SpawnCore();
            }
        }
        return;
    }

    if (m_animator)
    {
        Animation* curAnim = m_animator->GetCurrent();
        if (curAnim && curAnim->IsFinished())
            OnAnimLineFinished();
    }
}

//Boss1은 따로 구현
void Boss1::Update()
{
    UpdateRender();

    if (!m_bossActive)
        return;

    if (m_isCooldown)
    {
        m_cooldownTimer += fDT;
        if (m_cooldownTimer >= m_cooldownDuration)
        {
            m_isCooldown = false;
            m_cooldownTimer = 0.f;
            StartRandomPattern();
        }
        return;
    }

    m_patternTimer += fDT;
    UpdatePattern();
}

void Boss1::Pattern1()
{
    m_fireTimer1 += fDT;
    if (m_fireTimer1 < 0.02f) return;
    m_fireTimer1 = 0.f;

    Vec2 center = GetPos();

    float rad = m_angle1 * (PI / 180.f);
    Vec2 dir = { cosf(rad), sinf(rad) };

    auto* proj = new BossProjectile;
    proj->SetPos(center);
    proj->SetSize({ 20.f, 20.f });
    proj->SetDir(dir);

    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);

    m_angle1 += 25.f;
    if (m_angle1 >= 360.f) m_angle1 -= 360.f;
    GET_SINGLE(SceneManager)->GetCurScene()->StartShake(1.f, 5.f);
    GET_SINGLE(ResourceManager)->Play(L"EnemyShoot");
}

void Boss1::Pattern2()
{
    m_fireTimer2 += fDT;
    if (m_fireTimer2 < 0.25f) return;
    m_fireTimer2 = 0.f;

    Vec2 center = GetPos();
    int count = 12;

    for (int i = 0; i < count; i++)
    {
        float ang = (m_angle2 * PI / 180.f) + (PI * 2.f) * ((float)i / count);
        Vec2 dir = { cosf(ang), sinf(ang) };

        auto* proj = new BossProjectile;
        proj->SetPos(center);
        proj->SetSize({ 20.f, 20.f });
        proj->SetDir(dir);

        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    }

    m_angle2 += 10.f;
    if (m_angle2 >= 360.f) m_angle2 -= 360.f;
    GET_SINGLE(SceneManager)->GetCurScene()->StartShake(1.f, 5.f);
    GET_SINGLE(ResourceManager)->Play(L"EnemyShoot");
}

void Boss1::Pattern3()
{
    if (!m_laserActive)
    {
        m_laserActive = true;
        GET_SINGLE(ResourceManager)->Play(L"Laser");
        auto* leftLaser = new LaserObject(true);
        leftLaser->SetPos({ 0.f, WINDOW_HEIGHT / 2.f });
        leftLaser->SetDir(1);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(leftLaser, Layer::LASER);

        auto* rightLaser = new LaserObject(false);
        rightLaser->SetPos({ (float)WINDOW_WIDTH - 20.f, WINDOW_HEIGHT / 2.f });
        rightLaser->SetDir(-1);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(rightLaser, Layer::LASER);
        GET_SINGLE(SceneManager)->GetCurScene()->StartShake(1.f, 5.f);
    }
}

void Boss1::EndPattern()
{
    m_patternCount++;

    if (m_patternCount >= m_maxPatternCount && !m_isDying)
    {
        StartDeathSequence();
    }
    else
    {
        m_isCooldown = true;
        m_curPattern = Boss1Pattern::NONE;
    }
}

void Boss1::StartDeathSequence()
{
    m_isDying = true;
    m_isCooldown = false;
    m_curPattern = Boss1Pattern::NONE;
    m_patternTimer = 0.f;

    if (m_animator)
    {
        if (m_pDeathTexture)
        {
            Vec2 slice = { 160.f, 160.f };
            Vec2 step = { 160.f, 0.f };
            m_animator->CreateAnimation(
                m_deathAnimName,
                m_pDeathTexture,
                { 0.f, 0.f },
                slice,
                step,
                10,
                0.1f
            );
        }

        m_animator->Play(m_deathAnimName, PlayMode::Once, 1, 1.f);
        GET_SINGLE(ResourceManager)->Play(L"BossCoreDestroy");
    }
}

void Boss1::SpawnCore()
{
    m_isCorePhase = true;

    auto* core = new Boss1Core(this);
    core->SetPos(GetPos());
    core->SetSize({ 1.f, 1.f });
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(core, Layer::BOSSCORE);
}

void Boss1::SetupAnimations()
{
    Vec2 sliceSize = { 160.f, 160.f };
    Vec2 step = { 160.f, 0.f };

    m_animator->CreateAnimation(
        L"boss_line0",
        m_pTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        10,
        0.1f
    );

    m_animator->CreateAnimation(
        L"boss_line1",
        m_pTexture,
        { 0.f, 160.f },
        sliceSize,
        step,
        10,
        0.1f
    );

    m_animator->CreateAnimation(
        L"boss_line2",
        m_pTexture,
        { 0.f, 320.f },
        sliceSize,
        step,
        10,
        0.1f
    );

    m_animator->CreateAnimation(
        L"boss_line3",
        m_pTexture,
        { 0.f, 480.f },
        sliceSize,
        step,
        10,
        0.1f
    );

    m_totalLines = 4;
}

void Boss1::PlayNextLine()
{
    if (!m_animator)
        return;

    if (m_currLine >= m_totalLines)
        return;

    std::wstring animName = L"boss_line" + std::to_wstring(m_currLine);
    m_animator->Play(animName, PlayMode::Once, 1, 1.f);
}

void Boss1::OnAnimLineFinished()
{
    if (!m_animator)
        return;

    Animation* cur = m_animator->GetCurrent();
    if (!cur || !cur->IsFinished())
        return;

    m_currLine = (m_currLine + 1) % m_totalLines;

    PlayNextLine();
}