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

    GuideSetting();

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

    m_moveGuide.Update();
    m_jumpGuide.Update();
    m_attackGuide.Update();
    m_dashGuide.Update();
    m_shieldGuide.Update();
}

void TitleScene::Render(HDC hdc)
{
    m_titleLabel.Render(hdc);

    m_btnStart.Render(hdc);
    m_btnSettings.Render(hdc);
    m_btnExit.Render(hdc);

    m_moveGuide.Render(hdc);
    m_jumpGuide.Render(hdc);
    m_attackGuide.Render(hdc);
    m_dashGuide.Render(hdc);
    m_shieldGuide.Render(hdc);
}

void TitleScene::GuideSetting()
{
    m_moveGuide.SetFontType(FontType::TITLE);
    m_moveGuide.SetFontColor(RGB(200, 220, 255));
    m_moveGuide.SetText
    (
        L"이동 : A / D"
    );
    m_moveGuide.SetSize({ 300.f, 200.f });
    m_moveGuide.SetPos({ WINDOW_WIDTH * 0.8f, 330.f });

    m_jumpGuide.SetFontType(FontType::TITLE);
    m_jumpGuide.SetFontColor(RGB(200, 220, 255));
    m_jumpGuide.SetText
    (
        L"점프 : Space"
    );
    m_jumpGuide.SetSize({ 300.f, 200.f });
    m_jumpGuide.SetPos({ WINDOW_WIDTH * 0.8f, 360.f });

    m_attackGuide.SetFontType(FontType::TITLE);
    m_attackGuide.SetFontColor(RGB(200, 220, 255));
    m_attackGuide.SetText
    (
        L"공격 : J"
    );
    m_attackGuide.SetSize({ 300.f, 200.f });
    m_attackGuide.SetPos({ WINDOW_WIDTH * 0.8f, 390.f });

    m_dashGuide.SetFontType(FontType::TITLE);
    m_dashGuide.SetFontColor(RGB(200, 220, 255));
    m_dashGuide.SetText
    (
        L"대시 : K"
    );
    m_dashGuide.SetSize({ 300.f, 200.f });
    m_dashGuide.SetPos({ WINDOW_WIDTH * 0.8f, 420.f });

    m_shieldGuide.SetFontType(FontType::TITLE);
    m_shieldGuide.SetFontColor(RGB(200, 220, 255));
    m_shieldGuide.SetText
    (
        L"실드 : L"
    );
    m_shieldGuide.SetSize({ 300.f, 200.f });
    m_shieldGuide.SetPos({ WINDOW_WIDTH * 0.8f, 450.f });
}
