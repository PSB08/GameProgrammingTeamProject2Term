#pragma once
#include "Object.h"

class Boss3;
class Texture;

class Button :
    public Object
{
public:
    Button(Boss3* owner);
    ~Button();
public:
    void Render(HDC _hdc) override;
public:
    virtual void EnterCollision(Collider* _other)override;
private:
    Boss3* m_owner;
    Texture* m_Texture;
};

