#pragma once
#include "Object.h"

class Texture;
class Collider;


class BIgBullet :
    public Object
{
public:
    BIgBullet();
    ~BIgBullet();

    void Update() override;
    void Render(HDC _hdc) override;

public:
    void SetDir(Vec2 _dir)
    {
        m_dir = _dir;
        m_dir.Normalize();
        m_launched = true;
    }
private:
    Texture* m_pTexture;
    Vec2  m_dir;

    float m_lifeTime = 0.f;

    bool  m_launched = false;
    const float m_maxLifeAfterLaunch = 1.5f;
};

