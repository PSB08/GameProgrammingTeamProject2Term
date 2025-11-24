#include "pch.h"
#include "Boss3.h"
#include "Boss3Core.h"
#include "ExploseProjectile.h"
#include "BossProjectile.h"
#include "SceneManager.h"
#include "Scene.h"

Boss3::Boss3()
    : m_isCorePhase(false),
    m_isStartPhase(true),
    m_fireTimer1(0.f),
    m_angle1(0.f),
    m_fireTimer2(0.f),
    m_angle2(0.f)
{     
    StartPattern();
} 

Boss3::~Boss3()
{
}

void Boss3::StartRandomPattern()
{
    cout << "이거뜨면 버그임...";
}

void Boss3::UpdatePattern()
{
    if (m_isCorePhase)
        return; // 코어가 나오면 패턴 완전 정지
    if (m_isStartPhase)
    {
        StartPattern();
        return;
    }

    if (m_startDelayTimer < m_startDelay)
    {
        m_startDelayTimer += fDT;
        return; // 아직 대기 중이므로 패턴 실행 X
    }

    switch (m_curPattern)
    {
    case Boss3Pattern::PATTERN1:
        Pattern1();
        if (m_patternTimer > 5.f)
        {
            EndPattern();
        }
        break;

    case Boss3Pattern::PATTERN2:
        Pattern2();
        if (m_patternTimer > 5.f)
        {
            EndPattern();
        }
        break;

    case Boss3Pattern::PATTERN3:
        Pattern3();
        if (m_patternTimer > 3.f)
        {
            EndPattern();
        }
        break;
    case Boss3Pattern::PATTERN4:
        Pattern4();
        if (m_patternTimer > 5.f)
        {
            EndPattern();
        }
        break;

    case Boss3Pattern::PATTERN5:
        Pattern5();
        if (m_patternTimer > 3.f)
        {
            EndPattern();
        }
        break;
    }
}

void Boss3::EndPattern()
{
    m_isCooldown = true;
    m_curPattern = Boss3Pattern::NONE;

    if (m_patternCount >= m_maxPatternCount)
    {
        SpawnCore();
    }
}

void Boss3::StartPattern()
{
    m_fireTimer1 += fDT;
    if (m_fireTimer1 < 0.02f) return;
    m_fireTimer1 = 0.f;
    
    Vec2 center = GetPos();

    Vec2 dir = {  };

    auto* proj = new ExploseProjectile;
    proj->SetPos(center);
    proj->SetSize({ 30.f, 30.f });
    proj->SetDir(dir);

    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    m_isStartPhase = false;
}

void Boss3::Pattern1()
{
    cout << "adsf";
}

void Boss3::Pattern2()
{
    cout << "adsf";
}

void Boss3::Pattern3()
{
    cout << "adsf";
}

void Boss3::Pattern4()
{
    cout << "adsf";
}

void Boss3::Pattern5()
{
    cout << "adsf";
}

void Boss3::SpawnCore()
{
    m_isCorePhase = true;

    auto* core = new Boss3Core(this);
    core->SetPos(GetPos());
    core->SetSize({ 100.f, 100.f });
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(core, Layer::BOSSCORE);
}
