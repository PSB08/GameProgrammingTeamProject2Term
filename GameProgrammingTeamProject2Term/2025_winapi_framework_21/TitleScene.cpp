#include "pch.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameData.h"
#include <iostream>

TitleScene::TitleScene()
    : m_btnStart(L"", nullptr)
    , m_btnSettings(L"", nullptr)
    , m_btnExit(L"", nullptr)
{
    
}

void TitleScene::Init()
{
    GET_SINGLE(ResourceManager)->Play(L"TITLEBGM");
    Texture* backNormal = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Normal");
    Texture* backHover = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Hover");

    m_titleLabel.SetFontType(FontType::TITLE_BIG);
    m_titleLabel.SetFontColor(RGB(255, 255, 255));
    m_titleLabel.SetText(L"Deadlock");
    m_titleLabel.SetSize({ 800.f, 140.f });
    m_titleLabel.SetPos({ WINDOW_WIDTH / 2.f, 120.f });

    float btnW = 420.f;
    float btnH = 70.f;
    float startY = 300.f;
    float gap = 50.f;

    m_btnStart = UIButton(L"시작", []()
        {
            GameData::GetInstance()->Clear();
            GET_SINGLE(ResourceManager)->StopAllSounds();
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"BossSelectScene");
        });
    m_btnStart.SetSize({ btnW, btnH });
    m_btnStart.SetPos({ WINDOW_WIDTH / 2.f, startY });

    m_btnSettings = UIButton(L"설정", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"SettingScene");
        });
    m_btnSettings.SetSize({ btnW, btnH });
    m_btnSettings.SetPos({ WINDOW_WIDTH / 2.f, startY + (btnH + gap) });

    m_btnExit = UIButton(L"나가기", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            PostQuitMessage(0);
        });
    m_btnExit.SetSize({ btnW, btnH });
    m_btnExit.SetPos({ WINDOW_WIDTH / 2.f, startY + 2 * (btnH + gap) });

    m_btnStart.SetBackgroundTexture(backNormal, backHover);
    m_btnSettings.SetBackgroundTexture(backNormal, backHover);
    m_btnExit.SetBackgroundTexture(backNormal, backHover);
}

void TitleScene::Update()
{
    m_btnStart.Update();
    m_btnSettings.Update();
    m_btnExit.Update();
}

void TitleScene::Render(HDC hdc)
{
    m_titleLabel.Render(hdc);
    m_btnStart.Render(hdc);
    m_btnSettings.Render(hdc);
    m_btnExit.Render(hdc);
}
