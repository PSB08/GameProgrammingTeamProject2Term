#pragma once
#include "BossBase.h"
#include "Animator.h"
#include "Enums.h" // Boss1Pattern 포함

class Boss1 : public BossBase<Boss1Pattern>
{
public:
    //생성자, 소멸자
    Boss1();
    ~Boss1();

private:
    // 패턴 선택
    void StartRandomPattern() override;

    // 패턴 실행
    void UpdatePattern() override;
    void UpdateRender();
    void Update() override;

    void EndPattern();

private:
    // ===== 개별 패턴 =====
    void Pattern1();   // 360도 탄막
    void Pattern2();   // 6방향 탄막
    void Pattern3();   // 레이저 수축
    void SpawnCore();  //패턴 다 쓰고 코어 소환하기

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

private:
    Animator* m_animator;

    int m_currLine;
    int m_totalLines;

    void SetupAnimations();
    void PlayNextLine();
    void OnAnimLineFinished();

    bool m_isDying;
    Texture* m_pDeathTexture;
    std::wstring m_deathAnimName;

    void StartDeathSequence();
};
