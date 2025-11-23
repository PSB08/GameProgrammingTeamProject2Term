#pragma once
#include "Object.h"

class Texture;

class ExploseProjectile :
    public Object
{
public:
    ExploseProjectile();
    ~ExploseProjectile();
    void Update() override;
    void Render(HDC _hdc) override;
    void Explose(int _value);
public:
    virtual void EnterCollision(Collider* _other)override;
public:
    void SetAngle(float _f)
    {
        m_angle = _f;
    }
    void SetDir(Vec2 _dir)
    {
        m_dir = _dir;
        m_dir.Normalize();
    }
private:
    Texture* m_pTexture;
    float m_angle;
    Vec2 m_dir;

    float m_lifeTime = 0.f;
    int ProjectileValue = 1;
    float m_maxLifeTime;
};

