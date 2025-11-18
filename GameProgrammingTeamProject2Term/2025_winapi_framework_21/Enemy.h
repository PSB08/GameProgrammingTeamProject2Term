#pragma once
#include "Object.h"
class Enemy :
    public Object
{
public:
    Enemy();
    ~Enemy();
public:
    virtual void EnterCollision(Collider* _other)override; 
    virtual void StayCollision(Collider* _other) override;
    virtual void ExitCollision(Collider* _other) override;
    void Render(HDC _hdc);
};

