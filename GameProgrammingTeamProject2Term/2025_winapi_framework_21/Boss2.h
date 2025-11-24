#pragma once
#include "BossBase.h"
#include "BossProjectile.h"
#include "Enums.h"

class Boss2 : public BossBase<Boss2Pattern>
{
public:
	Boss2();
	~Boss2();

private:
    // 패턴 선택
    void StartRandomPattern() override;

    // 패턴 실행
    void UpdatePattern() override;

    void EndPattern();

private:
    // ===== 개별 패턴 =====
    void Pattern1();   // 
    void Pattern2();   // 
    void Pattern3();   // 
    void Pattern4();   // 
    void Pattern5();   // 
    void SpawnMainCore();  //패턴 다 쓰고 코어 소환하기
    void InitSpawnCore();


private:
    // 패턴용 변수들
    bool m_isCorePhase;
    float m_startDelayTimer = 0.f;  // 시작 지연용 타이머
    const float m_startDelay = 3.f;  //대기 타이머 - 3초 대기 후 패턴 시작함

    float m_angle1;
    float m_fireTimer1;

    float m_angle2;
    float m_fireTimer2;

    // 레이저 패턴용
    float m_laserLeftX;
    float m_laserRightX;
    bool  m_laserActive;
};

