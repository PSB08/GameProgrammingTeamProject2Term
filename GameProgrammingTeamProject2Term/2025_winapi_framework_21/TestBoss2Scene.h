#pragma once
#include "Scene.h"
class TestBoss2Scene :
    public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Init() override;
	void Update() override;
};

