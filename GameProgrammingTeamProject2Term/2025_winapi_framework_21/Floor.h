#pragma once
#include "Object.h"
class Floor : public Object
{
public:
    Floor();
    ~Floor();
public:
    void Update() override;
    void Render(HDC _hdc) override;
    
};

