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

    // 보스에서 호출해서 코어 열기
    void OpenCore();
    bool IsOpened() const { return m_isOpened; }

    void SetHP(int hp) { m_hp = hp; m_maxHp = hp; }

private:
    void HandleDeath();
    void SetupAnimations();  // 애니메이션 생성
    void ResetToIdle();      // 완전 닫힌 상태로 복귀

    enum class CoreState
    {
        Idle,       // 닫힘
        Opening,    // 열리는 중
        Opened,     // 열림 (파괴 가능)
        Returning,  // 다시 재생성
        Breaking    // 총알 맞고 파괴될 때
    };

private:
    Boss2* m_owner;
    int m_index;

    bool m_isOpened;     // 열렸다 플래그
    bool m_collidable;   // 플레이어 총알 충돌 허용 여부

    int m_hp;
    int m_maxHp;

    Texture* m_pTexture = nullptr;        // 일반 상태
    Texture* m_pDeathTexture = nullptr;        // 죽을 때
    Texture* m_pBrokenTexture = nullptr;  // 열려 있을 때 
    Texture* m_pBrokingTexture = nullptr;  // 부서 질 때
    Texture* m_pReturnTexture = nullptr;  // 돌아갈 때 
    Collider* m_collider;
    Animator* m_animator;

    CoreState m_state;

    // 열려있는 상태 유지 타이머
    float m_openTimer;
    float m_openDuration; // 5.f
};
