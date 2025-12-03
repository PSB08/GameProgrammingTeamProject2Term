#pragma once
#include "Object.h"
#include "Texture.h"

class Floor : public Object
{
public:
    Floor();
    ~Floor();
public:
    void Update() override;
    void Render(HDC _hdc) override;
private:
    Texture* m_texFloor = nullptr;
};

