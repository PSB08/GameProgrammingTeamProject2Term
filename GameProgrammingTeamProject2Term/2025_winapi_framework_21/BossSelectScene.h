#pragma once
#include "Scene.h"
#include "UIButton.h"
#include <memory>
#include <iostream>

class BossSelectScene : public Scene
{
public:
	BossSelectScene();
	~BossSelectScene();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override {}
    virtual void FixedUpdate(float) override {}
    virtual void Render(HDC hdc) override;
    virtual void Release() override {}

private:
    UIObject m_titleLabel;
    UIButton m_btnBoss1;
    UIButton m_btnBoss2;
    UIButton m_btnBoss3;
    UIButton m_btnTitle;
};

