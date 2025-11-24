#pragma once
#include "Scene.h"
#include "UIButton.h"
#include <memory>
#include <iostream>

class TitleScene : public Scene
{
public:
    TitleScene();
    virtual ~TitleScene() = default;

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override {}
    virtual void FixedUpdate(float) override {}
    virtual void Render(HDC hdc) override;
    virtual void Release() override {}

private:
    UIObject m_titleLabel;
    UIButton m_btnStart;
    UIButton m_btnSettings;
    UIButton m_btnExit;
};
