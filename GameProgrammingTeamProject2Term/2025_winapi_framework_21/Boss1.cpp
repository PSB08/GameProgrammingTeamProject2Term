#include "pch.h"
#include "Boss1.h"
#include "BossProjectile.h"
#include "SceneManager.h"
#include "Scene.h"

Boss1::Boss1()
    : m_angle1(0.f)
    , m_angle2(0.f)
    , m_fireTimer1(0.f)
    , m_fireTimer2(0.f)
    , m_laserLeftX(0.f)
    , m_laserRightX(WINDOW_WIDTH)
    , m_laserActive(false)
{
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
    switch (m_curPattern)
    {
    case Boss1Pattern::PATTERN1:
        Pattern1();
        if (m_patternTimer > 5.f)
        {
            m_isCooldown = true;
            m_curPattern = Boss1Pattern::NONE;
        }
        break;

    case Boss1Pattern::PATTERN2:
        Pattern2();
        if (m_patternTimer > 5.f)
        {
            m_isCooldown = true;
            m_curPattern = Boss1Pattern::NONE;
        }
        break;

    case Boss1Pattern::PATTERN3:
        Pattern3();
        if (m_patternTimer > 3.f)
        {
            m_laserActive = false;
            m_isCooldown = true;
            m_curPattern = Boss1Pattern::NONE;
        }
        break;
    }
}

void Boss1::RenderPattern(HDC _hdc)
{
    if (m_laserActive)
    {
        RECT leftLaser = { (LONG)m_laserLeftX, 0, (LONG)(m_laserLeftX + 20), WINDOW_HEIGHT };
        RECT rightLaser = { (LONG)(m_laserRightX - 20), 0, (LONG)m_laserRightX, WINDOW_HEIGHT };

        HBRUSH b = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(_hdc, &leftLaser, b);
        FillRect(_hdc, &rightLaser, b);
        DeleteObject(b);
    }
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
}

void Boss1::Pattern2()
{
    m_fireTimer2 += fDT;
    if (m_fireTimer2 < 0.25f) return;
    m_fireTimer2 = 0.f;

    Vec2 center = GetPos();
    constexpr int count = 12;

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
}

void Boss1::Pattern3()
{
    if (!m_laserActive)
    {
        m_laserActive = true;
        m_laserLeftX = 0.f;
        m_laserRightX = WINDOW_WIDTH;
    }

    m_laserLeftX += 400.f * fDT;
    m_laserRightX -= 400.f * fDT;
}

