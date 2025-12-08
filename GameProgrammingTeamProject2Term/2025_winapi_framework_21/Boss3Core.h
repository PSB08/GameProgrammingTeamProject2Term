#pragma once
#include "Enemy.h"
#include "Animator.h"
#include "Texture.h"

class Boss3;

class Boss3Core :
    public Enemy
{
public:
    Boss3Core(Boss3* owner);
    ~Boss3Core();

    virtual void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;

private:
    Boss3* m_owner; // ÆÄ±«ÇÒ Boss
    Texture* m_boss3Core;
    Animator* m_animator;

    bool m_pendingSceneChange = false;
    float m_delay = 0.f;
};

