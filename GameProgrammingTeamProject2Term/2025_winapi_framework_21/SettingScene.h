#pragma once
#include "Scene.h"
#include "UIButton.h"
#include "UISlider.h"
#include "Texture.h"

class SettingScene : public Scene
{
private:
    UIButton m_btnBack;

    UIObject m_lblBGM;
    UIObject m_lblEffect;

    UISlider m_sliderBGM;
    UISlider m_sliderEffect;

    UIObject m_imgBGMIcon;       // BGM 아이콘
    UIObject m_imgEffectIcon;    // 효과음 아이콘

    Texture* m_texVolumeOn;
    Texture* m_texVolumeOff;

public:
    SettingScene();
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};
