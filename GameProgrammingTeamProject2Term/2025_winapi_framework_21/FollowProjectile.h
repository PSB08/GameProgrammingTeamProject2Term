#pragma once
#include "Object.h"

class Texture;

class FollowProjectile :
    public Object
{
public:
    FollowProjectile();
    ~FollowProjectile();
    void Update() override;
    void Render(HDC _hdc) override;
    void Explose();
    void GetParentPosition();
    void GetPlayerPosition();
public:
    virtual void EnterCollision(Collider* _other) override;
public:
    void SetAngle(float _f)
    {
        m_angle = _f;
    }
    void SetDir(Vec2 _dir)
    {
        m_playerPos = _dir;
        m_playerPos.Normalize();
    }
private:
    Texture* m_pTexture;
    float m_angle;
    Vec2 m_playerPos;
    Vec2 m_myPos;
private:
    float m_lifeTime = 0.f;
    float m_timer = 0.f;
    float m_waitTimerValue;
    float m_exploseTimer = 0.f;
    float m_exploseTimerValue;
    bool m_exploseState = false;
    bool m_isfired = false;
};

