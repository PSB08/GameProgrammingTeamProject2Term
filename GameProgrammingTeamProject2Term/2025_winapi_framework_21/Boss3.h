#pragma once
#include "BossBase.h"
class Boss3 :
    public BossBase<Boss2Pattern>
{
public:
    //생성자, 소멸자
    Boss3();
    ~Boss3();

private:
    // 패턴 실행
    void UpdatePattern() override;

    void EndPattern();

private:
    // ===== 개별 패턴 =====
    void StartPattern(); // 개막패턴 -
    void Pattern1();   // 
    void Pattern2();   // 
    void Pattern3();   // 
    void Pattern4();   // 
    void Pattern5();   // 
    void SpawnCore();  // 기믹 클리어 후 코어 오픈

private:
    // 패턴용 변수들
    bool m_isCorePhase;

    float m_angle1;
    float m_fireTimer1;

    float m_angle2;
    float m_fireTimer2;

    // 레이저 패턴용
    float m_laserLeftX;
    float m_laserRightX;
    bool  m_laserActive;
};

