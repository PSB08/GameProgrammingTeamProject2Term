#include "pch.h"
#include "Boss2.h"
#include "Boss2Core.h"
#include "Boss2MainCore.h"
#include "Boss2Laser.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include <cmath>

Boss2::Boss2()
    : m_isCorePhase(false)
    , m_startDelayTimer(0.f)
    , m_startDelay(3.f)
    , m_angle1(0.f)
    , m_fireTimer1(0.f)
    , m_angle2(0.f)
    , m_fireTimer2(0.f)
    , m_patternInitialized(false)
    , m_patternElapsed(0.f)
{
    InitSpawnCore();
    //StartRandomPattern();
}

Boss2::~Boss2()
{
}

void Boss2::ResetPatternState()
{
    m_patternInitialized = false;
    m_fireTimer1 = 0.f;
    m_fireTimer2 = 0.f;

    // Pattern1
    m_pattern1Bullets.clear();
    m_pattern1Launched = false;

    // Pattern2
    if (m_laser)
    {
        m_laser->SetDead();
        m_laser = nullptr;
    }

    // Pattern3
    m_areaSpawned = false;
    m_areaLaunched = false;
    m_areaProjectiles.clear();

    // Pattern4
    for (auto& rb : m_pattern4RingBullets)
    {
        if (rb.proj)
            rb.proj->SetDead();
    }
    m_pattern4RingBullets.clear();
    m_pattern4Launched = false;

    // Pattern5
    m_pattern5SpawnedCount = 0;
}

void Boss2::SpawnCircleProjectiles(int count, float radius, float speed, const Vec2& center)
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    for (int i = 0; i < count; ++i)
    {
        float t = (2.f * PI * i) / (float)count;
        float dx = std::cos(t);
        float dy = std::sin(t);

        Vec2 dir = { dx, dy };

        Vec2 pos;
        pos.x = center.x + dx * radius;
        pos.y = center.y + dy * radius;

        BossProjectile* proj = new BossProjectile();
        proj->SetPos(pos);
        proj->SetSize({ 16.f, 16.f });
        proj->SetSpeed(speed);
        proj->Launch(dir);

        scene->AddObject(proj, Layer::BOSSPROJECTILE);
    }
}

void Boss2::StartRandomPattern()
{
    if (m_isCoreExplosionPhase)
        return;

    ResetPatternState();

    int r = rand() % 4;

    switch (r)
    {
    case 0: m_curPattern = Boss2Pattern::PATTERN1; break;
    case 1: m_curPattern = Boss2Pattern::PATTERN2; break;
    case 2: m_curPattern = Boss2Pattern::PATTERN4; break;
    case 3: m_curPattern = Boss2Pattern::PATTERN5; break;
    }

    m_patternTimer = 0.f;
    m_patternElapsed = 0.f;
}

void Boss2::UpdatePattern()
{
    if (m_isCorePhase)
        return;

    UpdateCorePositions();

    if (m_firstPattern)
    {
        m_firstPattern = false;

        m_startDelayTimer = 0.f;
        m_patternTimer = 0.f;
        m_patternElapsed = 0.f;

        ResetPatternState();
        StartRandomPattern();
        return;
    }

    if (m_startDelayTimer < m_startDelay)
    {
        m_startDelayTimer += fDT;
        return;
    }

    m_patternElapsed += fDT;

    // 패턴 실행
    switch (m_curPattern)
    {
    case Boss2Pattern::PATTERN1:
        Pattern1();
        break;
    case Boss2Pattern::PATTERN2:
        Pattern2();
        break;
    case Boss2Pattern::PATTERN3:
        Pattern3();
        break;
    case Boss2Pattern::PATTERN4:
        Pattern4();
        break;
    case Boss2Pattern::PATTERN5:
        Pattern5();
        break;
    }

    // 패턴 종료 체크
    switch (m_curPattern)
    {
    case Boss2Pattern::PATTERN1:
        if (m_patternElapsed > 5.f) EndPattern();
        break;
    case Boss2Pattern::PATTERN2:
        if (m_patternElapsed > 4.f) EndPattern();
        break;
    case Boss2Pattern::PATTERN3:
        if (m_patternElapsed > 3.f) EndPattern();
        break;
    case Boss2Pattern::PATTERN4:
        if (m_patternElapsed > 4.f) EndPattern();
        break;
    case Boss2Pattern::PATTERN5:
        if (m_patternElapsed > 4.f) EndPattern();
        break;
    }
}

void Boss2::UpdateCorePositions()
{
    Vec2 bossPos = GetPos();

    for (size_t i = 0; i < m_cores.size() && i < m_coreOffsets.size(); ++i)
    {
        Boss2Core* core = m_cores[i];
        if (!core) continue;

        Vec2 offset = m_coreOffsets[i];

        Vec2 pos;
        pos.x = bossPos.x + offset.x;
        pos.y = bossPos.y + offset.y;

        core->SetPos(pos);
    }
}

// ======================= PATTERN 1 =======================

void Boss2::Pattern1()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();
    Vec2 center = GetPos();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_fireTimer1 = 0.f;
    }

    if (m_patternElapsed < m_pattern1PrepareTime)
    {
        m_fireTimer1 += fDT;

        if (m_fireTimer1 >= m_pattern1SpawnInterval)
        {
            m_fireTimer1 = 0.f;

            float angle = ((float)rand() / RAND_MAX) * 2.f * PI;
            float radius = 50.f + ((float)rand() / RAND_MAX) * 250.f;

            float dx = std::cos(angle);
            float dy = std::sin(angle);

            Vec2 pos;
            pos.x = center.x + dx * radius;
            pos.y = center.y + dy * radius;

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 14.f, 14.f });
            proj->SetSpeed(380.f);
            scene->AddObject(proj, Layer::BOSSPROJECTILE);
            m_pattern1Bullets.push_back(proj);
        }
    }

    if (!m_pattern1Launched && m_patternElapsed > m_pattern1LaunchTime)
    {
        m_pattern1Launched = true;

        for (BossProjectile* proj : m_pattern1Bullets)
        {
            if (!proj) continue;
            Vec2 pos = proj->GetPos();
            Vec2 dir;
            dir.x = pos.x - center.x;
            dir.y = pos.y - center.y;
            proj->Launch(dir);
        }

        m_pattern1Bullets.clear();
    }
}

// ======================= PATTERN 2 =======================
// Boss2Laser 한 개만 사용, 내부에서 십자 레이저 회전

void Boss2::Pattern2()
{
    Vec2 center = GetPos();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;

        m_laser = new Boss2Laser();
        m_laser->SetPos(center);
        m_laser->SetArmLength(500.f);
        m_laser->SetThickness(40.f);
        m_laser->SetRotateSpeed(m_pattern2RotateSpeed);

        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(m_laser, Layer::LASER);
    }

    if (m_laser)
    {
        m_laser->SetPos(center);
    }
}

// ======================= PATTERN 3 =======================

void Boss2::Pattern3()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();
    Vec2 center = m_areaCenter;

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_areaSpawned = false;
        m_areaLaunched = false;
        m_areaProjectiles.clear();
    }

    if (!m_areaSpawned)
    {
        m_areaSpawned = true;

        int count = 40;
        for (int i = 0; i < count; ++i)
        {
            float t = (2.f * PI * i) / (float)count;
            float dx = std::cos(t);
            float dy = std::sin(t);

            Vec2 pos;
            pos.x = center.x + dx * m_areaRadius;
            pos.y = center.y + dy * m_areaRadius;

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 12.f, 12.f });
            proj->SetSpeed(400.f);
            scene->AddObject(proj, Layer::BOSSPROJECTILE);
            m_areaProjectiles.push_back(proj);
        }
    }

    if (!m_areaLaunched && m_patternElapsed > m_areaWarningDuration)
    {
        m_areaLaunched = true;

        for (BossProjectile* proj : m_areaProjectiles)
        {
            if (!proj) continue;
            Vec2 pos = proj->GetPos();
            Vec2 dir;
            dir.x = center.x - pos.x;
            dir.y = center.y - pos.y;
            proj->Launch(dir);
        }
    }
}

// ======================= PATTERN 4 =======================
// 보스 주변 링 탄이 2초 동안 두 바퀴 회전한 뒤 그 방향으로 발사

void Boss2::Pattern4()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();
    Vec2 center = GetPos();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_pattern4RingBullets.clear();
        m_pattern4Launched = false;

        int   count = 40;
        float radius = m_pattern4Radius;

        for (int i = 0; i < count; ++i)
        {
            float angle = (2.f * PI * i) / (float)count;

            float dx = std::cos(angle);
            float dy = std::sin(angle);

            Vec2 pos;
            pos.x = center.x + dx * radius;
            pos.y = center.y + dy * radius;

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 12.f, 12.f });
            proj->SetSpeed(420.f);
            scene->AddObject(proj, Layer::BOSSPROJECTILE);

            RingBullet rb;
            rb.proj = proj;
            rb.angle = angle;
            rb.radius = radius;
            m_pattern4RingBullets.push_back(rb);
        }
    }

    if (!m_pattern4Launched && m_patternElapsed < m_pattern4Duration)
    {
        for (auto& rb : m_pattern4RingBullets)
        {
            if (!rb.proj) continue;

            rb.angle += m_pattern4AngularSpeed * fDT;

            float dx = std::cos(rb.angle);
            float dy = std::sin(rb.angle);

            Vec2 pos;
            pos.x = center.x + dx * rb.radius;
            pos.y = center.y + dy * rb.radius;

            rb.proj->SetPos(pos);
        }
    }
    else if (!m_pattern4Launched)
    {
        m_pattern4Launched = true;

        for (auto& rb : m_pattern4RingBullets)
        {
            if (!rb.proj) continue;

            Vec2 pos = rb.proj->GetPos();
            Vec2 dir;
            dir.x = pos.x - center.x;
            dir.y = pos.y - center.y;
            rb.proj->Launch(dir);
        }
    }
}

// ======================= PATTERN 5 =======================
// 화면 모서리에서 직선 랜덤 탄막 (최대 탄 수 제한)

void Boss2::Pattern5()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_fireTimer1 = 0.f;
        m_pattern5SpawnedCount = 0;
    }

    if (m_pattern5SpawnedCount >= m_pattern5MaxBullets)
        return;

    m_fireTimer1 += fDT;

    if (m_fireTimer1 >= m_pattern5ShotInterval)
    {
        m_fireTimer1 = 0.f;

        int dirType = rand() % 4;

        // 한 번에 2발씩
        for (int i = 0; i < 2 && m_pattern5SpawnedCount < m_pattern5MaxBullets; ++i)
        {
            Vec2 pos;
            Vec2 dir;

            switch (dirType)
            {
            case 0: // 좌 -> 우
                pos = { 0.f, (float)(rand() % WINDOW_HEIGHT) };
                dir = { 1.f, 0.f };
                break;
            case 1: // 우 -> 좌
                pos = { (float)WINDOW_WIDTH, (float)(rand() % WINDOW_HEIGHT) };
                dir = { -1.f, 0.f };
                break;
            case 2: // 위 -> 아래
                pos = { (float)(rand() % WINDOW_WIDTH), 0.f };
                dir = { 0.f, 1.f };
                break;
            case 3: // 아래 -> 위
            default:
                pos = { (float)(rand() % WINDOW_WIDTH), (float)WINDOW_HEIGHT };
                dir = { 0.f, -1.f };
                break;
            }

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 10.f, 10.f });
            proj->SetSpeed(450.f);
            proj->Launch(dir);

            scene->AddObject(proj, Layer::BOSSPROJECTILE);

            ++m_pattern5SpawnedCount;
        }
    }
}

// ======================= 패턴 종료 / 코어 연동 =======================

void Boss2::EndPattern()
{
    Boss2Pattern finished = m_curPattern;

    m_patternCount++;
    m_patternsSinceLastCore++;

    m_isCooldown = true;
    m_curPattern = Boss2Pattern::NONE;

    if (finished == Boss2Pattern::PATTERN3)
    {
        m_isCoreExplosionPhase = false;
        ResetPatternState();
        m_patternTimer = 0.f;
        m_patternElapsed = 0.f;
        StartRandomPattern();
        return;
    }

    if (m_patternsSinceLastCore >= 3 && m_nextCoreToOpen < (int)m_cores.size())
    {
        m_patternsSinceLastCore = 0;

        Boss2Core* core = m_cores[m_nextCoreToOpen];
        if (core)
            core->OpenCore();

        ++m_nextCoreToOpen;
    }

    if (m_totalCoresDestroyed >= (int)m_cores.size())
    {
        SpawnMainCore();
    }
    else
    {
        ResetPatternState();
        m_patternTimer = 0.f;
        m_patternElapsed = 0.f;
        StartRandomPattern();
    }
}

void Boss2::NotifyCoreDestroyed(Boss2Core* core)
{
    m_totalCoresDestroyed++;

    m_isCoreExplosionPhase = true;
    m_curPattern = Boss2Pattern::PATTERN3;
    m_patternTimer = 0.f;
    m_patternElapsed = 0.f;

    //m_startDelayTimer = m_startDelay;

    if (core)
        m_areaCenter = core->GetPos();
    else
        m_areaCenter = GetPos();

    ResetPatternState();
}

void Boss2::SpawnMainCore()
{
    m_isCorePhase = true;

    auto* core = new Boss2MainCore(this);
    core->SetPos(GetPos());
    core->SetSize({ 100.f, 100.f });
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(core, Layer::BOSSCORE);
}

// 서브 코어 4개 생성

void Boss2::InitSpawnCore()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    m_cores.clear();
    m_coreOffsets.clear();

    m_nextCoreToOpen = 0;
    m_patternsSinceLastCore = 0;
    m_totalCoresDestroyed = 0;

    float offset = 450.f;
    float yOffset = 100.f;

    Vec2 offsets[4] =
    {
        { -offset, -yOffset }, // 좌상
        { -offset,  yOffset }, // 좌하
        {  offset, -yOffset }, // 우상
        {  offset,  yOffset }  // 우하
    };

    Vec2 bossPos = GetPos();

    for (int i = 0; i < 4; ++i)
    {
        Boss2Core* core = new Boss2Core(this, i);

        Vec2 pos;
        pos.x = bossPos.x + offsets[i].x;
        pos.y = bossPos.y + offsets[i].y;

        core->SetPos(pos);
        core->SetSize({ 60.f, 60.f });
        core->SetHP(10);

        scene->AddObject(core, Layer::BOSSCORE);

        m_cores.push_back(core);
        m_coreOffsets.push_back(offsets[i]);
    }
}
