#pragma once
#include "Scene.h"
#include "Texture.h"

class Boss2Scene :
    public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Init() override;
	void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Texture* m_pBackground = nullptr;
};

