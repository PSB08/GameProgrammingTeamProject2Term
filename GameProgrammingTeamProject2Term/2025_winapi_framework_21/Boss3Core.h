#pragma once
#include "Enemy.h"

class Boss3;

class Boss3Core :
    public Enemy
{
public:
    Boss3Core(Boss3* owner);
    ~Boss3Core();

    virtual void Update() override;
    void EnterCollision(Collider* _other) override;

private:
    Boss3* m_owner; // ÆÄ±«ÇÒ Boss

    bool m_pendingSceneChange = false;
    float m_delay = 0.f;
};

