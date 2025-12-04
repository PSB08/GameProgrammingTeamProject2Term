#include "pch.h"
#include "DeadScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>

DeadScene::DeadScene()
    : m_btnRetry(L"", nullptr)
    , m_btnTitle(L"", nullptr)
    , m_btnQuit(L"", nullptr)
{
}

void DeadScene::Init()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    GET_SINGLE(ResourceManager)->Play(L"PlayerDie");
    Texture* backNormal = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Normal");
    Texture* backHover = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Hover");

    m_titleLabel.SetFontType(FontType::TITLE_BIG);
    m_titleLabel.SetFontColor(RGB(255, 255, 255));
    m_titleLabel.SetText(L"게임 오버");
    m_titleLabel.SetSize({ 800.f, 140.f });
    m_titleLabel.SetPos({ WINDOW_WIDTH / 2.f, 120.f });

    float btnW = 420.f;
    float btnH = 70.f;
    float startY = 300.f;
    float gap = 50.f;

    m_btnRetry = UIButton(L"다시 시작", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->LoadScene(L"BossSelectScene");
        });
    m_btnRetry.SetSize({ btnW, btnH });
    m_btnRetry.SetPos({ WINDOW_WIDTH / 2.f, startY });

    m_btnTitle = UIButton(L"타이틀로", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");
        });
    m_btnTitle.SetSize({ btnW, btnH });
    m_btnTitle.SetPos({ WINDOW_WIDTH / 2.f, startY + (btnH + gap) });

    m_btnQuit = UIButton(L"나가기", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            PostQuitMessage(0);
        });
    m_btnQuit.SetSize({ btnW, btnH });
    m_btnQuit.SetPos({ WINDOW_WIDTH / 2.f, startY + 2 * (btnH + gap) });

    m_btnRetry.SetBackgroundTexture(backNormal, backHover);
    m_btnTitle.SetBackgroundTexture(backNormal, backHover);
    m_btnQuit.SetBackgroundTexture(backNormal, backHover);
}

void DeadScene::Update()
{
    m_btnRetry.Update();
    m_btnTitle.Update();
    m_btnQuit.Update();
}

void DeadScene::Render(HDC hdc)
{
    m_titleLabel.Render(hdc);
    m_btnRetry.Render(hdc);
    m_btnTitle.Render(hdc);
    m_btnQuit.Render(hdc);
}
