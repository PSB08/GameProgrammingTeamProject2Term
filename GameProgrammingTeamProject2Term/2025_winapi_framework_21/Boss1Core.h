#pragma once
#include "Enemy.h"

class Boss1;

class Boss1Core : public Enemy
{
public:
    Boss1Core(Boss1* owner);
    ~Boss1Core();

    void EnterCollision(Collider* _other) override;

private:
    Boss1* m_owner; // ÆÄ±«ÇÒ Boss
};
