#include "pch.h"
#include "DeadScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>

DeadScene::DeadScene()
    : m_btnTitle(L"", nullptr)
    , m_btnQuit(L"", nullptr)
{
}

void DeadScene::Init()
{
    m_titleLabel.SetFontType(FontType::TITLE_BIG);
    m_titleLabel.SetText(L"게임 오버");
    m_titleLabel.SetSize({ 800.f, 140.f });
    m_titleLabel.SetPos({ WINDOW_WIDTH / 2.f, 120.f });

    float btnW = 420.f;
    float btnH = 70.f;
    float startY = 300.f;
    float gap = 50.f;

    m_btnTitle = UIButton(L"타이틀로", []()
        {
            GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");
        });
    m_btnTitle.SetSize({ btnW, btnH });
    m_btnTitle.SetPos({ WINDOW_WIDTH / 2.f, startY + (btnH + gap) });

    m_btnQuit = UIButton(L"나가기", []()
        {
            PostQuitMessage(0);
        });
    m_btnQuit.SetSize({ btnW, btnH });
    m_btnQuit.SetPos({ WINDOW_WIDTH / 2.f, startY + 2 * (btnH + gap) });
}

void DeadScene::Update()
{
    m_btnTitle.Update();
    m_btnQuit.Update();
}

void DeadScene::Render(HDC hdc)
{
    m_titleLabel.Render(hdc);
    m_btnTitle.Render(hdc);
    m_btnQuit.Render(hdc);
}
