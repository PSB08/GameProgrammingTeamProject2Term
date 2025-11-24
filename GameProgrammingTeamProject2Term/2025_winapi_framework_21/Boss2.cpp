#include "pch.h"
#include "Boss2.h"
#include "Boss2MainCore.h"
#include "BossProjectile.h"
#include "LaserObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Player.h"

Boss2::Boss2()
    : m_angle1(0.f)
    , m_angle2(0.f)
    , m_fireTimer1(0.f)
    , m_fireTimer2(0.f)
    , m_laserLeftX(0.f)
    , m_laserRightX(WINDOW_WIDTH)
    , m_laserActive(false)
    , m_isCorePhase(false)
{
    InitSpawnCore();
    StartRandomPattern();
}

Boss2::~Boss2()
{
}

void Boss2::StartRandomPattern()
{
    int r = 0;//rand() % 5;  //이 랜덤 방식도 바뀌어야 함 -> 코어 파괴 때 3이 실행되어야 하기 때문.
    switch (r)
    {
        case 0: m_curPattern = Boss2Pattern::PATTERN1; break;
        case 1: m_curPattern = Boss2Pattern::PATTERN2; break;
        case 2: m_curPattern = Boss2Pattern::PATTERN3; break;
        case 3: m_curPattern = Boss2Pattern::PATTERN4; break;
        case 4: m_curPattern = Boss2Pattern::PATTERN5; break;
    }

    m_patternTimer = 0.f;
}

void Boss2::UpdatePattern()
{
    if (m_isCorePhase)
        return;

    if (m_startDelayTimer < m_startDelay)
    {
        m_startDelayTimer += fDT;
        return;
    }

    switch (m_curPattern)
    {
    case Boss2Pattern::PATTERN1:
        Pattern1();
        if (m_patternTimer > 5.f)  //미정
        {
            EndPattern();
        }
        break;

    case Boss2Pattern::PATTERN2:
        Pattern2();
        if (m_patternTimer > 5.f)  //미정
        {
            EndPattern();
        }
        break;

    case Boss2Pattern::PATTERN3:
        Pattern3();
        if (m_patternTimer > 3.f)  //미정
        {
            m_laserActive = false;
            EndPattern();
        }
        break;

    case Boss2Pattern::PATTERN4:
        Pattern4();
        if (m_patternTimer > 3.f)  //미정
        {
            m_laserActive = false;
            EndPattern();
        }
        break;

    case Boss2Pattern::PATTERN5:
        Pattern5();
        if (m_patternTimer > 3.f)  //미정
        {
            m_laserActive = false;
            EndPattern();
        }
        break;
    }
}

void Boss2::Pattern1()
{
    //총알을 맵에 뿌린 뒤, 몇 초후에 플레이어 방향으로 총알이 날아옴
}

void Boss2::Pattern2()
{
    //나태지옥 패턴 - 레이저를 4 방향 배치, 계속 돌아감
}

void Boss2::Pattern3()
{
    //코어 파괴 때 실행하는 패턴, 역장 생성(범위 표시) 후 자폭 패턴
}

void Boss2::Pattern4()
{
    //맵 전체에 폭탄 대기 -> 코어 근처에 방어막 생성(범위 표시)되고 안에 들어가면 세이프
}

void Boss2::Pattern5()
{
    //좌, 우에서 총알 y값 무작위로 발사 or 위, 아래에서 총알 x값 무작위로 발사 패턴
}

void Boss2::EndPattern()
{
    m_patternCount++;
    m_isCooldown = true;
    m_curPattern = Boss2Pattern::NONE;

    if (m_patternCount >= m_maxPatternCount)
    {
        SpawnMainCore();
    }
}

void Boss2::SpawnMainCore()
{
    m_isCorePhase = true;

    auto* core = new Boss2MainCore(this);
    core->SetPos(GetPos());
    core->SetSize({ 100.f, 100.f });
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(core, Layer::BOSSCORE);
}

void Boss2::InitSpawnCore()
{
    //Boss2MainCore 이런 느낌으로 Boss2Core를 만들고 보스 기준 좌상단, 좌하단, 우상단, 우하단에 배치
    //이 코어는 보스가 패턴을 몇 번 쓰면 하나씩 열림 -> 3번에 코어 하나, 12번하면 코어 모두 파괴,
    //라스트 보스 패턴 3가지 발생, Boss2MainCore가 다시 보스 위치에 생성, 파괴하면 끝 -> 이 코어는 Boss1Core랑 똑같이 작동
    //m_patternCount랑 m_maxPatternCount는 BossBase에 존재, Boss2는 따로 만들 예정임 -> 작동 방식이 다르기 때문

}
