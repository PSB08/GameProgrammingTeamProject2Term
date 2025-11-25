#pragma once
#include "Object.h"
class DeadFloor : public Object
{
public:
    DeadFloor();
    ~DeadFloor();
public:
    void Update() override;
    void Render(HDC _hdc) override;
};

