#pragma once
#include "Object.h"

class Texture;

class BossProjectile :
    public Object
{
public:
    BossProjectile();
    ~BossProjectile();
    void Update() override;
    void Render(HDC _hdc) override;
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

    void Launch(const Vec2& dir)
    {
        m_dir = dir;
        m_dir.Normalize();
        m_launched = true;
        m_lifeTime = 0.f; // 발사 후 생명타이머 재시작
    }

    void SetSpeed(float s) { m_speed = s; }

    // 상태 조회
    bool IsLaunched() const { return m_launched; }

private:
    Texture* m_pTexture;
    float m_angle;
    Vec2 m_dir;

    float m_lifeTime = 0.f;

    bool  m_launched = false;
    float m_speed = 500.f;
    const float m_maxLifeAfterLaunch = 1.5f;
};

