#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animator.h"

class Boss1;

class Boss1Core : public Enemy
{
public:
    Boss1Core(Boss1* owner);
    ~Boss1Core();

    virtual void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;

private:
    void SetupAnimations();

private:
    Texture* m_pTexture;
    Animator* m_animator;

    Boss1* m_owner; // ÆÄ±«ÇÒ Boss

    bool m_pendingSceneChange = false;
    float m_delay = 0.f;
};
