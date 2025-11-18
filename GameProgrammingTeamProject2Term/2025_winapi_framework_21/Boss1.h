#pragma once
#include "BossBase.h"
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

    // 필요하면 추가 렌더링
    void RenderPattern(HDC _hdc) override;

private:
    // ===== 개별 패턴 =====
    void Pattern1();   // 360도 탄막
    void Pattern2();   // 6방향 탄막
    void Pattern3();   // 레이저 수축

private:
    // 패턴용 변수들
    float m_angle1;
    float m_fireTimer1;

    float m_angle2;
    float m_fireTimer2;

    // 레이저 패턴용
    float m_laserLeftX;
    float m_laserRightX;
    bool  m_laserActive;
};
