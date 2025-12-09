#pragma once
#include "Object.h"

class Texture;


class Explosion :
    public Object
{
public:
    Explosion();
    ~Explosion();
public:
    void Update() override;
    void Render(HDC _hdc) override;
private:
    float m_timer = 0.f;
    float m_explosingTime;
    Texture* m_ExpTexture;
};

