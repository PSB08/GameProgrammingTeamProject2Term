#pragma once
#include "BossBase.h"
#include "Enums.h"
#include "Player.h"
#include "Button.h"
#include "Animator.h"

class Texture;

class Boss3 : public BossBase<Boss3Pattern>
{
public:
    //생성자, 소멸자
    Boss3();
    ~Boss3();

private:
    void StartRandomPattern() override;
    // 패턴 실행
    void UpdatePattern() override;

    void EndPattern();
    void Render(HDC _hdc) override;

private:
    // ===== 개별 패턴 =====
    void StartPattern(); // 개막패턴 -
    void Pattern1();   // 위치 옮기기
    void Pattern2();   // 
    void Pattern3();   // 
    void Pattern4();   // 
    void Pattern5();   // 
    void SpawnCore();  // 기믹 클리어 후 코어 오픈
    void CheckAnimationEnd(std::wstring _animationName, bool repeat = false);
public:
    void PressedButton();
    void StartDeathSequence();
private:
    std::vector<Button*> m_Buttons;
    Texture* m_BATexture;
    Texture* m_BFTexture;
    Texture* m_BUFTexture;
    Texture* m_BDTexture;
    Animator* m_animator;
private:
    // 패턴용 변수들
    bool m_isCorePhase;
    bool m_isStartPhase;
    float m_shotDealy = 0.f;
    float m_startDelayTimer = 0.f;  // 시작 지연용 타이머
    const float m_startDelay = 3.f;  //대기 타이머 - 3초 대기 후 패턴 시작함
    bool m_setPos = false;
    bool m_isShotFollow = false;
    bool m_doShake = false;
    bool m_doFire = false;
    bool m_isDying = false;

    float m_angle1;
    float m_fireTimer1;

    float m_angle2;
    float m_fireTimer2;
    
    float m_position = 0.f;


};

