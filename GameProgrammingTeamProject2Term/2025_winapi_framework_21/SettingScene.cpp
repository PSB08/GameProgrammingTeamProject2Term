#include "pch.h"
#include "SettingScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"

SettingScene::SettingScene()
    : m_btnBack(L"", nullptr)
{
}

void SettingScene::Init()
{
    float centerX = WINDOW_WIDTH / 2.f;

    m_lblBGM.SetFontType(FontType::TITLE);
    m_lblBGM.SetText(L"BGM Volume");
    m_lblBGM.SetPos({ centerX - 220.f, 200.f });
    m_lblBGM.SetSize({ 200.f, 40.f });

    m_sliderBGM.SetPos({ centerX + 100.f, 200.f });
    m_sliderBGM.SetSize({ 300.f, 30.f });

    float bgmVol = 1.f;
    GET_SINGLE(ResourceManager)->m_pChannel[(UINT)SOUND_CHANNEL::BGM]->getVolume(&bgmVol);
    m_sliderBGM.SetValue(bgmVol);

    m_lblEffect.SetFontType(FontType::TITLE);
    m_lblEffect.SetText(L"Effect Volume");
    m_lblEffect.SetPos({ centerX - 220.f, 300.f });
    m_lblEffect.SetSize({ 200.f, 40.f });

    m_sliderEffect.SetPos({ centerX + 100.f, 300.f });
    m_sliderEffect.SetSize({ 300.f, 30.f });

    float effVol = 1.f;
    GET_SINGLE(ResourceManager)->m_pChannel[(UINT)SOUND_CHANNEL::EFFECT]->getVolume(&effVol);
    m_sliderEffect.SetValue(effVol);

    m_btnBack = UIButton(L"뒤로가기", []()
        {
            GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");
        });

    m_btnBack.SetSize({ 300.f, 60.f });
    m_btnBack.SetPos({ centerX, 500.f });
}

void SettingScene::Update()
{
    m_btnBack.Update();
    m_sliderBGM.Update();
    m_sliderEffect.Update();

    GET_SINGLE(ResourceManager)->Volume(SOUND_CHANNEL::BGM, m_sliderBGM.GetValue());
    GET_SINGLE(ResourceManager)->Volume(SOUND_CHANNEL::EFFECT, m_sliderEffect.GetValue());
}

void SettingScene::Render(HDC hdc)
{
    m_lblBGM.Render(hdc);
    m_lblEffect.Render(hdc);

    m_sliderBGM.Render(hdc);
    m_sliderEffect.Render(hdc);

    m_btnBack.Render(hdc);
}
