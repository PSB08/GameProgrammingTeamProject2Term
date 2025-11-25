#pragma once
#include "Object.h"

class Texture;
class Collider;

class BossProjectile : public Object
{
public:
    BossProjectile();
    ~BossProjectile();

    void Update() override;
    void Render(HDC _hdc) override;

public:
    void SetAngle(float _f) { m_angle = _f; }

    void SetDir(Vec2 _dir)
    {
        m_dir = _dir;
        m_dir.Normalize();
        m_launched = true;
    }

    // 방향 설정 + 발사 상태 전환
    void Launch(const Vec2& dir)
    {
        m_dir = dir;
        m_dir.Normalize();
        m_launched = true;
        m_lifeTime = 0.f;
    }

    void SetSpeed(float s) { m_speed = s; }
    bool IsLaunched() const { return m_launched; }

    // 레이저 경고 / 실제 공격 구분용
    void SetColliderName(const std::wstring& name);

private:
    Texture* m_pTexture;
    Collider* m_collider;

    float m_angle;
    Vec2  m_dir;

    float m_lifeTime = 0.f;

    bool  m_launched = false;
    float m_speed = 500.f;
    const float m_maxLifeAfterLaunch = 1.5f;
};
