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
    void SetGravity(bool _value);
    void SetDivision(bool _value);
    void SetRigid(bool _value);
    void SetValue(int _value);
    void SetForce(Vec2 _force);
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
    Vec2 m_power;

    bool m_gravityExplosion = false;
    bool m_endDivision = false;
    bool m_useRigid = true;
    float m_lifeTime = 0.f;
    int ProjectileValue = 1;
    float m_maxLifeTime;
    float m_delayTime = 0.f;
    int m_ExploseValue = 12;
};

