#pragma once
#include "Enemy.h"

class Boss2;
class Texture;
class Collider;
class Animator;

class Boss2Core : public Enemy
{
public:
    Boss2Core(Boss2* owner, int index);
    virtual ~Boss2Core();

    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* _other) override;

    // 보스에서 호출해서 “코어 열기” (이때부터 3초 카운트 시작)
    void OpenCore();
    bool IsOpened() const { return m_isOpened; }

    void SetHP(int hp) { m_hp = hp; m_maxHp = hp; }

private:
    void HandleDeath();      // HP 0 → 완전 파괴 (돌아가지 않음)
    void SetupAnimations();  // 애니메이션 생성
    void ResetToIdle();      // 완전 닫힌 상태로 복귀

    enum class CoreState
    {
        Idle,       // 닫힘 (맞을 수 없음)
        Opened,     // 열림 (파괴 가능, 3초 타이머 진행)
        Returning,  // 3초 버티고 살아남아서 돌아가는 애니 중
        Breaking    // 총알 맞고 파괴될 때 애니 중 (끝나면 SetDead)
    };

private:
    Boss2* m_owner;
    int m_index;

    bool m_isOpened;     // “열렸다” 플래그
    bool m_collidable;   // 플레이어 총알 충돌 허용 여부

    int m_hp;
    int m_maxHp;

    Texture* m_pTexture = nullptr;        // 일반 상태
    Texture* m_pBrokenTexture = nullptr;  // 열려 있을 때 
    Texture* m_pBrokingTexture = nullptr;  // 부서 질 때
    Texture* m_pReturnTexture = nullptr;  // 돌아갈 때 
    Collider* m_collider;
    Animator* m_animator;

    CoreState m_state;

    // 열려있는 상태 유지 타이머 (열린 순간부터 3초 세기)
    float m_openTimer;
    float m_openDuration; // 5.f
};
