#pragma once
#include "BossBase.h"
#include "BossProjectile.h"
#include "Enums.h"
#include <vector>

class Boss2Core;
class Boss2Laser;

class Boss2 : public BossBase<Boss2Pattern>
{
public:
    Boss2();
    ~Boss2();

    // 코어가 파괴될 때 호출 (Boss2Core -> Boss2)
    void NotifyCoreDestroyed(Boss2Core* core);

private:
    // BossBase 인터페이스
    void StartRandomPattern() override;
    void UpdatePattern() override;
    void EndPattern();

    // 패턴
    void Pattern1();   // 보스 주변에 탄 깔고, 나중에 바깥 방향으로 발사
    void Pattern2();   // 십자 레이저 (Boss2Laser, 회전)
    void Pattern3();   // 코어 폭발 시 범위 자폭
    void Pattern4();   // 보스 주변 링 탄이 2초 동안 2바퀴 회전 → 그 방향으로 발사
    void Pattern5();   // 화면 모서리에서 직선 랜덤 탄막

    void SpawnMainCore();   // 서브 코어 전부 파괴 시 메인 코어 등장
    void InitSpawnCore();   // 서브 코어 생성
    void UpdateCorePositions(); // 보스 위치에 따라 서브 코어 위치 갱신

    // 유틸
    void ResetPatternState();
    void SpawnCircleProjectiles(int count, float radius, float speed, const Vec2& center);

private:
    // 기본 상태
    bool  m_isCorePhase;

    float m_startDelayTimer = 0.f;
    const float m_startDelay = 3.f;

    float m_angle1;
    float m_fireTimer1;

    float m_angle2;
    float m_fireTimer2;

    bool  m_patternInitialized = false;

    // Pattern1
    std::vector<BossProjectile*> m_pattern1Bullets;
    bool  m_pattern1Launched = false;
    float m_pattern1PrepareTime = 1.5f;
    float m_pattern1LaunchTime = 2.0f;
    float m_pattern1SpawnInterval = 0.03f;

    // Pattern2 (레이저)
    Boss2Laser* m_laser = nullptr;
    float       m_pattern2RotateSpeed = 2; // Boss2Laser에 전달할 회전 속도

    // Pattern4 (회전 후 발사)
    struct RingBullet
    {
        BossProjectile* proj;
        float angle;
        float radius;
    };
    std::vector<RingBullet> m_pattern4RingBullets;
    bool  m_pattern4Launched = false;
    float m_pattern4Duration = 2.f;                // 2초 동안 회전 (2바퀴)
    float m_pattern4AngularSpeed = 2.f * PI;  // 초당 1바퀴
    float m_pattern4Radius = 160.f;

    // Pattern5
    float m_pattern5ShotInterval = 0.12f;
    int   m_pattern5SpawnedCount = 0;
    int   m_pattern5MaxBullets = 120;  // 패턴당 최대 탄 수

    // 코어 관리
    std::vector<Boss2Core*> m_cores;
    std::vector<Vec2>       m_coreOffsets;
    int   m_nextCoreToOpen = 0;
    int   m_patternsSinceLastCore = 0;
    int   m_totalCoresDestroyed = 0;

    // Pattern3(코어 폭발 범위) 공통
    Vec2  m_areaCenter;
    float m_areaRadius = 200.f;
    float m_areaWarningDuration = 1.5f;
    bool  m_areaSpawned = false;
    bool  m_areaLaunched = false;
    bool  m_isCoreExplosionPhase = false;
    std::vector<BossProjectile*> m_areaProjectiles;

private:
    bool m_firstPattern = true;
    float m_patternElapsed = 0.f;
};
