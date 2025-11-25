#include "pch.h"
#include "Boss2.h"
#include "Boss2Core.h"
#include "Boss2MainCore.h"
#include "Boss2Laser.h"
#include "LaserObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"
#include <cmath>

Boss2::Boss2()
    : m_isCorePhase(false)
    , m_startDelayTimer(0.f)
    , m_startDelay(3.f)
    , m_fireTimer1(0.f)
    , m_patternInitialized(false)
    , m_patternElapsed(0.f)
{
    InitSpawnCore();
}

Boss2::~Boss2()
{
}

//초기화
void Boss2::ResetPatternState()
{
    m_patternInitialized = false;
    m_fireTimer1 = 0.f;

    // Pattern1
    m_pattern1Bullets.clear();
    m_pattern1Launched = false;

    // Pattern2
    for (auto& v : m_pattern2VerticalLasers)
    {
        if (v.laser)
            v.laser->SetDead();
    }
    m_pattern2VerticalLasers.clear();
    m_pattern2SideWaveCount = 0;
    m_pattern2VerticalWaveCount = 0;

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

    #pragma region Patterns

//PATTERN 1 총알 막 뿌리고 한 번에 발사
void Boss2::Pattern1()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();
    Vec2 center = GetPos();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_fireTimer1 = 0.f;
    }

    if (m_patternElapsed < m_pattern1PrepareTime)  //1.5초 동안 생성하기
    {
        m_fireTimer1 += fDT;

        if (m_fireTimer1 >= m_pattern1SpawnInterval)  //0.03초 마다 생성하기 
        {
            m_fireTimer1 = 0.f;

            float angle = ((float)rand() / RAND_MAX) * 2.f * PI;  //angle을 360도에서 랜덤 뽑을려고 2ㅠ 곱함
            float radius = 50.f + ((float)rand() / RAND_MAX) * 250.f;  //떨어진거리

            float dx = std::cos(angle);
            float dy = std::sin(angle);  //여기가 벡터 구하기 

            Vec2 pos;
            pos.x = center.x + dx * radius;
            pos.y = center.y + dy * radius;  //Vec2로 점 생성하기

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 14.f, 14.f });
            proj->SetSpeed(380.f);
            scene->AddObject(proj, Layer::BOSSPROJECTILE);
            m_pattern1Bullets.push_back(proj);
            //총알 생성 후 vector에 추가하기
        }
    }

    if (!m_pattern1Launched && m_patternElapsed > m_pattern1LaunchTime)  //2초후에
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
            //발사하기
        }

        m_pattern1Bullets.clear(); //혹시 모르니까 clear까지
    }
}

//PATTERN 2 레이저 생성하고 오는 패턴
void Boss2::Pattern2()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;

        m_pattern2SideWaveCount = 0;
        m_pattern2VerticalWaveCount = 0;
        m_pattern2VerticalLasers.clear();
    }  //그냥 초기화

    if (m_pattern2SideWaveCount < 1)
    {
        bool canSpawn = false;

        if (m_pattern2SideWaveCount == 0 && m_patternElapsed >= 0.f)  //시작하자마자 수직 레이저 발사
            canSpawn = true;
        else if (m_pattern2SideWaveCount == 1 && m_patternElapsed >= m_pattern2SideInterval)  //대기 후 발사
            canSpawn = true;

        if (canSpawn)
        {
            auto* leftLaser = new LaserObject(true);
            leftLaser->SetPos({ 0.f, WINDOW_HEIGHT / 2.f });
            leftLaser->SetDir(1);
            scene->AddObject(leftLaser, Layer::LASER);

            auto* rightLaser = new LaserObject(false);
            rightLaser->SetPos({ (float)WINDOW_WIDTH - 20.f, WINDOW_HEIGHT / 2.f });
            rightLaser->SetDir(-1);
            scene->AddObject(rightLaser, Layer::LASER);

            ++m_pattern2SideWaveCount;
        }  //여기까지가 생성
    }

    if (m_pattern2VerticalWaveCount < 1)
    {
        float baseStart =
            m_pattern2VerticalStartTime +
            m_pattern2VerticalWaveCount * m_pattern2VerticalInterval;
        //수평 레이저 
        if (m_patternElapsed >= baseStart)
        {
            {
                Boss2Laser* laser = new Boss2Laser();
                laser->SetPos({ WINDOW_WIDTH / 2.f, -50.f });
                laser->SetArmLength(WINDOW_WIDTH / 2.f);
                laser->SetThickness(60.f);
                scene->AddObject(laser, Layer::LASER);

                VerticalLaser v;
                v.laser = laser;
                v.dir = 1;
                m_pattern2VerticalLasers.push_back(v);  //생성하고 VerticalLaser에 추가
            }

            {
                Boss2Laser* laser = new Boss2Laser();
                laser->SetPos({ WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT + 50.f });
                laser->SetArmLength(WINDOW_WIDTH / 2.f);
                laser->SetThickness(60.f);
                scene->AddObject(laser, Layer::LASER);

                VerticalLaser v;
                v.laser = laser;
                v.dir = -1;
                m_pattern2VerticalLasers.push_back(v);  //여기도 마찬가지
            }

            ++m_pattern2VerticalWaveCount;
        }
    }

    for (size_t i = 0; i < m_pattern2VerticalLasers.size(); )
    {
        VerticalLaser& v = m_pattern2VerticalLasers[i];  //Verticallaser를 움직이기

        if (!v.laser)
        {
            m_pattern2VerticalLasers.erase(m_pattern2VerticalLasers.begin() + i);
            continue;
        }

        v.laser->Translate({ 0.f, v.dir * m_pattern2VerticalSpeed * fDT });

        float y = v.laser->GetPos().y;
        bool shouldDie = false;

        if (v.dir > 0)
        {
            if (y > WINDOW_HEIGHT / 2.f)
                shouldDie = true;  //중앙이면 삭제
        }
        else
        {
            if (y < WINDOW_HEIGHT / 2.f)
                shouldDie = true;  //중앙이면 삭제
        }

        if (shouldDie)
        {
            v.laser->SetDead();
            m_pattern2VerticalLasers.erase(m_pattern2VerticalLasers.begin() + i);  //삭제
        }
        else
        {
            ++i;
        }
    }
}

//PATTERN 3 서브 코어 터지면 총알 발사하는 패턴
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
    }  //초기화

    if (!m_areaSpawned)
    {
        m_areaSpawned = true;  //스폰 되어 있지 않으면 스폰 하기

        int count = 40;
        for (int i = 0; i < count; ++i)
        {
            float t = (2.f * PI * i) / (float)count;
            float dx = std::cos(t);
            float dy = std::sin(t);  //총 40발을 360도 돌면서 생성하기

            Vec2 pos;
            pos.x = center.x + dx * m_areaRadius;
            pos.y = center.y + dy * m_areaRadius;

            BossProjectile* proj = new BossProjectile();
            proj->SetPos(pos);
            proj->SetSize({ 12.f, 12.f });
            proj->SetSpeed(400.f);
            scene->AddObject(proj, Layer::BOSSPROJECTILE);
            m_areaProjectiles.push_back(proj);  //생성하고 넣기
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
        }  //발사 시간되면 발사하기
    }
}

//PATTERN 4 보스 위치에서 총알 발사하는 패턴
void Boss2::Pattern4()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();
    Vec2 center = GetPos();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_pattern4RingBullets.clear();
        m_pattern4Launched = false;  //초기화

        int count = 40;
        float radius = m_pattern4Radius;

        for (int i = 0; i < count; ++i)
        {
            float angle = (2.f * PI * i) / (float)count;

            float dx = std::cos(angle);
            float dy = std::sin(angle);  //총알 생성, 방향 설정

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
            m_pattern4RingBullets.push_back(rb);  //진짜 생성, 넣기
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

            rb.proj->SetPos(pos);  //2바퀴 회전
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
            dir.x = pos.x - center.x;  //보스 중심에서 바깥으로 향하는 벡터
            dir.y = pos.y - center.y;
            rb.proj->Launch(dir);  //회전 끝나면 방향으로 발사
        }
    }
}

//PATTERN 5 x, y값 랜덤 위치에서 총알 막 날라오는 패턴
void Boss2::Pattern5()
{
    std::shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurScene();

    if (!m_patternInitialized)
    {
        m_patternInitialized = true;
        m_fireTimer1 = 0.f;
        m_pattern5SpawnedCount = 0;
    }  //초기화

    if (m_pattern5SpawnedCount >= m_pattern5MaxBullets)
        return;  //최대 탄 수 넘으면 return

    m_fireTimer1 += fDT;

    if (m_fireTimer1 >= m_pattern5ShotInterval)
    {
        m_fireTimer1 = 0.f;  //발사 타이밍 마다 발사함

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

            ++m_pattern5SpawnedCount;  //발사하고 총알 수++
        }
    }
}

    #pragma endregion

    #pragma region Core

//패턴 종료 / 코어 생성
void Boss2::EndPattern()
{
    Boss2Pattern finished = m_curPattern;

    m_patternCount++;  //총 패턴 몇 번 했는지 추가
    m_patternsSinceLastCore++;  //서브 코어 파괴 후 패턴 몇 번 했는지 추가

    m_isCooldown = true;
    m_curPattern = Boss2Pattern::NONE;  //패턴 없애기

    if (finished == Boss2Pattern::PATTERN3)  //근데 3이 끝난거면
    {
        m_isCoreExplosionPhase = false;
        ResetPatternState();
        m_patternElapsed = 0.f;
        StartRandomPattern();
        return;  //초기화 작업
    }

    if (m_patternsSinceLastCore >= 3 && m_nextCoreToOpen < (int)m_cores.size())  //만약 3번 했으면
    {
        m_patternsSinceLastCore = 0;  //초기화

        Boss2Core* core = m_cores[m_nextCoreToOpen];
        if (core)
            core->OpenCore();  //서브 코어 열기

        ++m_nextCoreToOpen;  //인덱스 추가
    }

    if (m_totalCoresDestroyed >= (int)m_cores.size())
    {
        SpawnMainCore();  //서브 코어 수 보다 부서진 수가 같거나 많아지면 메인 소환
    }
    else  //아니면 초기화
    {
        ResetPatternState();
        m_patternElapsed = 0.f;
        StartRandomPattern();
    }
}

void Boss2::NotifyCoreDestroyed(Boss2Core* core)
{
    m_totalCoresDestroyed++;  //파괴 수 증가

    m_isCoreExplosionPhase = true;
    m_curPattern = Boss2Pattern::PATTERN3;
    m_patternElapsed = 0.f;  //패턴 3으로 설정

    if (core)
        m_areaCenter = core->GetPos();
    else
        m_areaCenter = GetPos();
    //센터를 core위치로 잡기
    ResetPatternState();  //초기화
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
    m_coreOffsets.clear();  //코어들 초기화, 위치 초기화

    m_nextCoreToOpen = 0;
    m_patternsSinceLastCore = 0;
    m_totalCoresDestroyed = 0;  //초기화

    float offset = 450.f;
    float yOffset = 100.f;  //x랑 y의 offset 설정

    Vec2 offsets[4] =
    {
        { -offset, -yOffset }, // 좌상
        { -offset,  yOffset }, // 좌하
        {  offset, -yOffset }, // 우상
        {  offset,  yOffset }  // 우하
    };
    //위치 잡기
    Vec2 bossPos = GetPos();

    for (int i = 0; i < 4; ++i)
    {
        Boss2Core* core = new Boss2Core(this, i);  //위치에 따라 생성

        Vec2 pos;
        pos.x = bossPos.x + offsets[i].x;
        pos.y = bossPos.y + offsets[i].y;

        core->SetPos(pos);
        core->SetSize({ 60.f, 60.f });
        core->SetHP(1);  //사실 체력 넣으려다가 뺌

        scene->AddObject(core, Layer::BOSSCORE);

        m_cores.push_back(core);
        m_coreOffsets.push_back(offsets[i]);  //코어랑 위치 넣기
    }

    for (size_t i = 0; i < m_cores.size(); ++i)
    {
        size_t j = rand() % m_cores.size();

        std::swap(m_cores[i], m_cores[j]);
        std::swap(m_coreOffsets[i], m_coreOffsets[j]);  //위치 랜덤 돌리기
    }

}

//코어 위치 업데이트
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

    #pragma endregion
