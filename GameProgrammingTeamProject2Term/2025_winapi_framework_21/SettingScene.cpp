#include "pch.h"
#include "SettingScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"

SettingScene::SettingScene()
    : m_btnBack(L"", nullptr)
    , m_texVolumeOn(nullptr)
    , m_texVolumeOff(nullptr)
{
}

void SettingScene::Init()
{
    float centerX = WINDOW_WIDTH / 2.f;

    //슬라이더 - 브금
    m_lblBGM.SetFontType(FontType::TITLE);
    m_lblBGM.SetFontColor(RGB(255, 255, 255));
    m_lblBGM.SetText(L"BGM Volume");
    m_lblBGM.SetPos({ centerX - 220.f, 200.f });
    m_lblBGM.SetSize({ 200.f, 40.f });

    m_sliderBGM.SetPos({ centerX + 100.f, 200.f });
    m_sliderBGM.SetSize({ 300.f, 30.f });

    float bgmVol = 1.f;
    GET_SINGLE(ResourceManager)->m_pChannel[(UINT)SOUND_CHANNEL::BGM]->getVolume(&bgmVol);
    m_sliderBGM.SetValue(bgmVol);

    //슬라이더 - 이펙트
    m_lblEffect.SetFontType(FontType::TITLE);
    m_lblEffect.SetFontColor(RGB(255, 255, 255));
    m_lblEffect.SetText(L"Effect Volume");
    m_lblEffect.SetPos({ centerX - 220.f, 300.f });
    m_lblEffect.SetSize({ 200.f, 40.f });

    m_sliderEffect.SetPos({ centerX + 100.f, 300.f });
    m_sliderEffect.SetSize({ 300.f, 30.f });

    float effVol = 1.f;
    GET_SINGLE(ResourceManager)->m_pChannel[(UINT)SOUND_CHANNEL::EFFECT]->getVolume(&effVol);
    m_sliderEffect.SetValue(effVol);

    //사운드 이미지
    m_imgBGMIcon.SetPos({ centerX - 100.f, 200.f });
    m_imgBGMIcon.SetSize({ 40.f, 40.f });

    m_imgEffectIcon.SetPos({ centerX - 100.f, 300.f });
    m_imgEffectIcon.SetSize({ 40.f, 40.f });

    m_texVolumeOn = GET_SINGLE(ResourceManager)->GetTexture(L"Speaker_On");
    m_texVolumeOff = GET_SINGLE(ResourceManager)->GetTexture(L"Speaker_Off");

    m_imgBGMIcon.m_texture = (bgmVol > 0.f) ? m_texVolumeOn : m_texVolumeOff;
    m_imgEffectIcon.m_texture = (effVol > 0.f) ? m_texVolumeOn : m_texVolumeOff;

    //나가기
    m_btnBack = UIButton(L"뒤로가기", []()
        {
            GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");
        });

    m_btnBack.SetSize({ 300.f, 60.f });
    m_btnBack.SetPos({ centerX, 500.f });

    Texture* backNormal = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Normal");
    Texture* backHover = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Hover");

    m_btnBack.SetBackgroundTexture(backNormal, backHover);
}

void SettingScene::Update()
{
    m_btnBack.Update();
    m_sliderBGM.Update();
    m_sliderEffect.Update();

    float bgm = m_sliderBGM.GetValue();
    float eff = m_sliderEffect.GetValue();

    GET_SINGLE(ResourceManager)->Volume(SOUND_CHANNEL::BGM, bgm);
    GET_SINGLE(ResourceManager)->Volume(SOUND_CHANNEL::EFFECT, eff);

    m_imgBGMIcon.m_texture = (bgm <= 0.f) ? m_texVolumeOff : m_texVolumeOn;
    m_imgEffectIcon.m_texture = (eff <= 0.f) ? m_texVolumeOff : m_texVolumeOn;
}

void SettingScene::Render(HDC hdc)
{
    m_lblBGM.Render(hdc);
    m_lblEffect.Render(hdc);

    m_imgBGMIcon.Render(hdc);
    m_imgEffectIcon.Render(hdc);

    m_sliderBGM.Render(hdc);
    m_sliderEffect.Render(hdc);

    m_btnBack.Render(hdc);
}
