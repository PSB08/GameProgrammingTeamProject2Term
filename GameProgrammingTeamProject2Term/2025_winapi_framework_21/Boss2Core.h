#pragma once
#include "Enemy.h"

class Boss2;
class Texture;
class Collider;

// 서브 코어
// - 처음엔 닫힘 상태(파란 테두리, 충돌 X)
// - Boss2::OpenCore() 호출 시 빨간색으로 바뀌고, 충돌 가능
class Boss2Core : public Enemy
{
public:
    Boss2Core(Boss2* owner, int index);
    virtual ~Boss2Core();

    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* _other) override;

    void OpenCore();            // 열어서 공격 가능 상태로 만들기
    bool IsOpened() const { return m_isOpened; }

    void SetHP(int hp) { m_hp = hp; }

private:
    void HandleDeath();

private:
    Boss2* m_owner;
    int       m_index;

    bool      m_isOpened;
    bool      m_isDead;
    bool      m_collidable;     // true 일 때만 충돌 처리

    int       m_hp;

    Texture* m_pTexture;
    Collider* m_collider;
};
