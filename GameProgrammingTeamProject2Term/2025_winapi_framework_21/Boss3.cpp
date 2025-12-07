#include "pch.h"
#include "Boss3.h"
#include "Boss3Core.h"
#include "ExploseProjectile.h"
#include "BossProjectile.h"
#include "SceneManager.h"
#include "BIgBullet.h"
#include "Scene.h"
#include "FollowProjectile.h"
#include "EventBus.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Animation.h"

Boss3::Boss3()
    : m_isCorePhase(false),
    m_isStartPhase(true),
    m_fireTimer1(0.f),
    m_angle1(0.f),
    m_fireTimer2(0.f),
    m_angle2(0.f)
{     

    AddComponent<Animator>();
    m_animator = GetComponent<Animator>();


    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss3");
    m_BATexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss3Attack");
    m_BFTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss3Fold");
    m_BUFTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss3UnFold");

    m_animator->CreateAnimation
    (L"Idle"
        , m_pTexture
        , { 0.f,0.f }
        , { 128.f,256.f }
        , { 128.f,0.f }
    , 5, 0.1f);
    m_animator->CreateAnimation
    (L"bossAttack"
        , m_BATexture
        , { 0.f,0.f }
        , { 128.f,256.f }
        , { 128.f,0.f }
    , 5, 0.1f);
    m_animator->CreateAnimation
    (L"Fold"
        , m_BFTexture
        , { 0.f,0.f }
        , { 128.f,256.f }
        , { 128.f,0.f }
    , 6, 0.1f);
    m_animator->CreateAnimation
    (L"UnFold"
        , m_BUFTexture
        , { 0.f,0.f }
        , { 128.f,256.f }
        , { 128.f,0.f }
    , 6, 0.1f);



    m_animator->Play(L"UnFold", PlayMode::Once, 1, 1.f);

    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    for (int i = 0; i < 3; ++i)
    {
        Button* button = new Button(this);  //위치에 따라 생성

        Vec2 pos;
        pos.x = 320 * (i + 1);

        button->SetPos(pos);
        button->SetSize({ 60.f, 60.f });

        scene->AddObject(button, Layer::BUTTON);
    }
    m_Buttons.clear();

    if (m_animator)
    {
        Animation* cur = m_animator->GetCurrent();
        if (cur && cur->IsFinished())
        {
            StartPattern();
        }
    }

} 

Boss3::~Boss3()
{
}

void Boss3::StartRandomPattern()
{
    m_setPos = false;
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
        if (m_patternTimer > 2.f)
        {
            EndPattern();
            m_curPattern = Boss3Pattern::PATTERN2;
        }
        break;

    case Boss3Pattern::PATTERN2:
        Pattern2();
        if (m_patternTimer > 3.f)
        {
            EndPattern();
            m_curPattern = Boss3Pattern::PATTERN3;
        }
        break;

    case Boss3Pattern::PATTERN3:
        Pattern3();
        cout << "P3";
        if (m_patternTimer > 1.5f)
        {
            EndPattern();
            m_curPattern = Boss3Pattern::PATTERN4;
        }
        break;
    case Boss3Pattern::PATTERN4:
        Pattern4();
        if (m_patternTimer > 2.f)
        {
            EndPattern();
            m_curPattern = Boss3Pattern::PATTERN5;
        }
        break;

    case Boss3Pattern::PATTERN5:
        Pattern5();
        if (m_patternTimer > 3.f)
        {
            EndPattern();
            m_curPattern = Boss3Pattern::PATTERN1;
        }
        break;
    }
}

void Boss3::EndPattern()
{
    m_animator->Play(L"UnFold", PlayMode::Once, 1, 1.f);
    m_isShotFollow = false;
    m_isCooldown = true;
    m_shotDealy = 0.f;
    m_patternTimer = 0.f;
    m_doShake = false;
    m_doFire = false;
    if (m_patternCount >= m_maxPatternCount)
    {
        SpawnCore();
    }
}

void Boss3::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss3::StartPattern()
{
    m_fireTimer1 += fDT;
    if (m_fireTimer1 < 1.f) return;
    m_fireTimer1 = 0.f;
    
    Vec2 center = GetPos();

    Vec2 dir = {  };

    auto* proj = new ExploseProjectile;
    proj->SetPos(center);
    proj->SetSize({ 30.f, 30.f });
    proj->SetDir(dir);
    proj->SetGravity(false);
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    m_isStartPhase = false;
    m_curPattern = Boss3Pattern::PATTERN1;
}

void Boss3::Pattern1()
{
    Vec2 pos = GetPos();
    float nowPosition = pos.x;

    while (!m_setPos)
    {
        int randPos = rand() % 3;
        switch (randPos)
        {
        case 0:
            m_position = 320.f;
            break;
        case 1:
            m_position = 640.f;
            break;
        case 2:
            m_position = 960.f;
            break;
        }

        if (nowPosition != m_position)
        {
            m_setPos = true;
            SetPos({ m_position, pos.y });
            break;
        }
    }
}

void Boss3::Pattern2()
{
    if (m_doFire == false)
    {
        m_animator->Play(L"bossAttack");
        m_doFire = true;
    }
    Vec2 center = GetPos();

    Vec2 dir = {  };

    if (!m_isShotFollow)
    {
        m_isShotFollow = true;
        auto* proj = new FollowProjectile;
        proj->SetPos(center);
        proj->SetSize({ 30.f, 30.f });
        proj->SetDir(dir);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    }
}

void Boss3::Pattern3()
{
    Vec2 playerPos;

    for (UINT i = 0; i < (UINT)Layer::END; ++i)
    {
        const auto& objects = GET_SINGLE(SceneManager)->GetCurScene()->GetLayerObjects((Layer)i);
        for (Object* obj : objects)
        {
            if (!obj)
                continue;
            if ((Layer)i == Layer::PLAYER)
                playerPos = obj->GetPos();
        }
    }

    if (m_doFire == false)
    {
        Vec2 pos = GetPos();
        SetPos({ playerPos.x, pos.y });
        m_animator->Play(L"UnFold", PlayMode::Once, 1, 1.f);
        m_doFire = true;
    }


    m_fireTimer1 += fDT;
    if (m_fireTimer1 < 1.f) return;
    m_fireTimer1 = 0.f;

    Vec2 center = GetPos();

    Vec2 dir = {  };

    for (int i = 0; i < 20; i++)
    {
        float dx = rand() % 80 + 10;
        float dy = rand() % 300 + 250;

        if(i % 2 == 0)
        dx *= -1;

        auto* proj = new ExploseProjectile;
        proj->SetPos(center);
        proj->SetSize({ 30.f, 30.f });
        proj->SetForce({dx, -dy});
        proj->SetGravity(true);
        proj->SetDivision(true);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    }
}

void Boss3::Pattern4()
{
    SetPos({ m_position, 140.f });
    if (m_doFire == false)
    {
        m_animator->Play(L"UnFold", PlayMode::Once, 1, 1.f);
        m_doFire = true;
    }
    else
    {
        CheckAnimationEnd(L"bossAttack", true);
    }


    m_shotDealy += fDT;
    if (m_shotDealy < 0.2f) return;
   
    float dx = rand() % 1000 - 500;
    int ran = rand() % 10 + 3;
    Vec2 center = GetPos();
    auto* proj = new ExploseProjectile;
    proj->SetPos(center);
    proj->SetSize({ 30.f, 30.f });
    proj->SetDir({ dx, 1000.f });
    proj->SetGravity(false);
    proj->SetDivision(true);
    proj->SetRigid(false);
    proj->SetValue(ran);
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);
    m_shotDealy = 0.f;
   
   
}

void Boss3::Pattern5()
{
    if (m_doShake == false)
    {
        m_animator->Play(L"bossAttack");
        GET_SINGLE(SceneManager)->GetCurScene()->StartShake(0.3f, 70.f);
        EventBus::Invoke(L"PlayerBounce");

        auto* proj = new BIgBullet;
        proj->SetPos({1400.f, 500.f});
        proj->SetSize({ 500.f, 500.f });
        proj->SetDir({-150.f, 500.f});
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(proj, Layer::BOSSPROJECTILE);

        m_doShake = true;
    }

}

void Boss3::SpawnCore()
{
    m_isCorePhase = true;

    auto* core = new Boss3Core(this);
    core->SetPos(GetPos());
    core->SetSize({ 100.f, 100.f });
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(core, Layer::BOSSCORE);
}

void Boss3::CheckAnimationEnd(std::wstring _animationName, bool repeat)
{
    if (m_animator)
    {
        Animation* cur = m_animator->GetCurrent();
        if (cur && cur->IsFinished())
        {
            if(repeat)
                m_animator->Play(_animationName, PlayMode::Loop);
            else
                m_animator->Play(_animationName, PlayMode::Once, 1, 1.f);
        }
    }
}

void Boss3::PressedButton()
{
    m_patternCount++;
}
