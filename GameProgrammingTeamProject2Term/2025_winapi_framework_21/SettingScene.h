#pragma once
#include "Scene.h"
#include "UIButton.h"
#include "UISlider.h"

class SettingScene : public Scene
{
private:
    UIButton m_btnBack;

    UIObject m_lblBGM;
    UIObject m_lblEffect;

    UISlider m_sliderBGM;
    UISlider m_sliderEffect;

public:
    SettingScene();
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};
