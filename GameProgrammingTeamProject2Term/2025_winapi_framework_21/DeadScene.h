#pragma once
#include "Scene.h"
#include "UIButton.h"
#include "pch.h"
#include <memory>
#include <iostream>

class DeadScene :
    public Scene
{
public:
    DeadScene();
    virtual ~DeadScene() = default;

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override {}
    virtual void FixedUpdate(float) override {}
    virtual void Render(HDC hdc) override;
    virtual void Release() override {}

private:
private:
    UIObject m_titleLabel;
    UIButton m_btnReStart;
    UIButton m_btnSettings;
    UIButton m_btnTitle;
};

