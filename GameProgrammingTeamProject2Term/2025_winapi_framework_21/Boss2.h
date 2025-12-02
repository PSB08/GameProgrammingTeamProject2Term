#pragma once
#include "BossBase.h"
#include "BossProjectile.h"
#include "Enums.h"
#include "Animator.h"
#include <vector>

class Boss2Core;
class Boss2Laser;
class LaserObject;

class Boss2 : public BossBase<Boss2Pattern>
{
public:
    Boss2();
    ~Boss2();

    //서브 코어가 파괴될 때 호출 (Boss2Core -> Boss2)
    //어떤 코어가 죽었는지 알려주고, 그 위치 기준으로 코어 폭발 패턴(PATTERN3)을 발동
    void NotifyCoreDestroyed(Boss2Core* core);
    void StartDeathSequence();

private:
    //랜덤으로 다음 패턴을 선택
    void StartRandomPattern() override;

    //매 프레임 패턴 갱신
    void UpdatePattern() override;

    //현재 패턴 종료 처리 + 다음 패턴/코어 오픈/메인 코어 소환 등
    void EndPattern();

    //각 패턴 구현
    void Pattern1();   //보스 주변에 랜덤 위치로 탄을 깔아두고 => 일정 시간 후 바깥 방향으로 전부 발사
    void Pattern2();   //LaserObject 좌/우에서 2번 + Boss2Laser 위/아래에서 2번 쓸고 지나가는 레이저 패턴
    void Pattern3();   //서브 코어 폭발 시 코어 위치 기준 원형 범위에서 안쪽으로 탄막 수축
    void Pattern4();   //보스 주변 링 탄이 2초 동안 두 바퀴 회전 => 회전이 끝난 방향으로 일제 발사
    void Pattern5();   //화면 네 모서리/변에서 직선으로 날아오는 랜덤 탄막

    //메인 코어 / 서브 코어 관리
    void SpawnMainCore();        //서브 코어 모두 파괴 시, 보스 본체는 패턴 멈추고 메인 코어 소환
    void InitSpawnCore();        //전투 시작 시 서브 코어 4개 생성
    void UpdateCorePositions();  //보스 위치가 움직이면, 그에 맞춰 서브 코어 위치를 offset 기준으로 재배치

    //패턴 공용 상태 리셋
    void ResetPatternState();

private:
    //메인 코어가 나와 있는가
    bool m_isCorePhase;

    //패턴 시작 전 대기 시간용 타이머
    float m_startDelayTimer = 0.f;

    //패턴 시작 전 고정 대기 시간(맨 처음에)
    const float m_startDelay = 3.f;

    //여러 패턴에서 공유하는 쿨타임 타이머
    float m_fireTimer1;

    //현재 패턴이 초기화를 이미 했는가
    bool m_patternInitialized = false;


private:
    //-- 패턴 1 --
    //PATTERN1에서 보스 주변에 깔아둔 탄들을 저장
    std::vector<BossProjectile*> m_pattern1Bullets;

    //PATTERN1에서 준비 단계 => 발사 단계로 넘어갔는가
    bool m_pattern1Launched = false;

    //PATTERN1 : 탄을 뿌려두는 준비 시간
    float m_pattern1PrepareTime = 1.5f;

    //PATTERN1 : 이 시간이 지나면 깔아둔 탄을 한 번에 발사
    float m_pattern1LaunchTime = 2.0f;

    //PATTERN1 : 준비 시간 동안 탄을 생성하는 간격
    float m_pattern1SpawnInterval = 0.03f;
    

private:
    //-- 패턴 2 --
    //위/아래에서 내려오거나 올라오는 Boss2Laser를 관리하기 위한 구조체
    struct VerticalLaser
    {
        Boss2Laser* laser = nullptr;
        int dir = 0;
    };

    //PATTERN2에서 생성된 위/아래 레이저들을 모두 저장
    std::vector<VerticalLaser> m_pattern2VerticalLasers;

    //좌/우에서 들어오는 레이저 세트를 몇 번 발사했는지
    int m_pattern2SideWaveCount = 0;

    //위/아래에서 들어오는 레이저 세트를 몇 번 발사했는지
    int m_pattern2VerticalWaveCount = 0;

    //위/아래 수평 레이저 속도
    float m_pattern2VerticalSpeed = 600.f;

    //좌/우 레이저 생성 사이 간격 시간
    float m_pattern2SideInterval = 1.2f;

    //위/아래 레이저를 처음 발사하기 시작하는 시간
    float m_pattern2VerticalStartTime = 2.0f;

    //위/아래 레이저 생성 사이 간격 시간
    float m_pattern2VerticalInterval = 1.2f;


private:
    //-- 패턴 4 --
    //보스 주변에 원형으로 배치된 탄 정보를 저장하는 구조체
    struct RingBullet
    {
        BossProjectile* proj;
        float angle;
        float radius;
    };

    //PATTERN4의 링 탄 목록
    std::vector<RingBullet> m_pattern4RingBullets;

    //PATTERN4에서 회전이 끝나고 탄을 발사했는지 여부
    bool m_pattern4Launched = false;

    //PATTERN4 : 링 탄이 회전하는 전체 지속 시간
    float m_pattern4Duration = 2.f;

    //PATTERN4 : 초당 회전 각속도
    float m_pattern4AngularSpeed = 2.f * PI;

    //PATTERN4 : 링의 반지름
    float m_pattern4Radius = 160.f;


private:
    //-- 패턴 5 --
    //탄 발사 간격
    float m_pattern5ShotInterval = 0.12f;

    //지금까지 생성한 탄 개수
    int m_pattern5SpawnedCount = 0;

    //PATTERN5에서 한 패턴 동안 생성 가능한 최대 탄 수
    int m_pattern5MaxBullets = 90;


private:
    //-- 패턴 3 --
    //서브 코어 객체 리스트
    std::vector<Boss2Core*> m_cores;

    //각 서브 코어가 보스 위치로부터 얼마나 떨어져 있는지
    std::vector<Vec2> m_coreOffsets;

    //다음에 열릴 서브 코어 인덱스
    int m_nextCoreToOpen = 0;

    // 마지막으로 서브 코어를 열고 난 이후, 몇 개의 패턴을 수행했는지
    int m_patternsSinceLastCore = 0;

    //지금까지 파괴된 서브 코어 수
    int m_totalCoresDestroyed = 0;

    //코어 폭발의 중심 위치
    Vec2 m_areaCenter;

    //폭발 범위의 반지름
    float m_areaRadius = 200.f;

    //이 시간이 지나면 안쪽으로 탄이 수축 발사됨
    float m_areaWarningDuration = 0.5f;

    //경고용 원형 탄을 이미 생성했는지 여부
    bool m_areaSpawned = false;

    //경고 후 실제로 탄을 발사했는지 여부
    bool m_areaLaunched = false;

    //코어 폭발 패턴(PATTERN3)이 진행 중인지 상태 플래그
    bool m_isCoreExplosionPhase = false;

    //코어 폭발 패턴에서 사용하는 원형 탄 목록
    std::vector<BossProjectile*> m_areaProjectiles;

private:
    //첫 패턴인지 여부
    bool m_firstPattern = true;

    //현재 패턴이 시작된 후 흐른 시간
    float m_patternElapsed = 0.f;

private:
        Animator* m_animator;  //애니메이터
        bool m_isDyingToCore;  //죽었는가
        Texture* m_pDeathTexture;  //죽었을 때 나오는 텍스처
        std::wstring m_deathAnimName;  //애니메이션 이름

        void SetupAnimations();  //애니메이션 세팅
        void PlayIdle();  //기본 애니메이션 시작

};
