#pragma once
#include "Enemy.h"

class Boss1;

class Boss1Core : public Enemy
{
public:
    Boss1Core(Boss1* owner);
    ~Boss1Core();

    virtual void Update() override;
    void EnterCollision(Collider* _other) override;

private:
    Boss1* m_owner; // ÆÄ±«ÇÒ Boss

    bool m_pendingSceneChange = false;
    float m_delay = 0.f;
};
