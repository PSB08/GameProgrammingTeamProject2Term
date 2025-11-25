#pragma once
#include "Scene.h"
class Boss3Scene :
    public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Init() override;
	void Update() override;
};

