#pragma once
#include "Scene.h"
class TestBossScene :
    public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Init() override;
	void Update() override;
};

