#pragma once
#include "Object.h"
class UpDownPlatform : public Object
{
public:
    UpDownPlatform();
    ~UpDownPlatform();
public:
    void Update() override;
    void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* _other)override;
private:
    float m_timer = 0.f;
    float m_DownCheckTimer;
    int m_UpCheckValue;
    int m_hitValue = 0;
    int m_nowSize = 1;
    float m_sizeDownValue;

};

