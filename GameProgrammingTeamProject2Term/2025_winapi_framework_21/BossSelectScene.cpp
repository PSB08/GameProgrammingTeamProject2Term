#include "pch.h"
#include "BossSelectScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>

BossSelectScene::BossSelectScene()
    : m_btnBoss1(L"", nullptr)
    , m_btnBoss2(L"", nullptr)
    , m_btnBoss3(L"", nullptr)
    , m_btnTitle(L"", nullptr)
{

}

BossSelectScene::~BossSelectScene()
{
}

void BossSelectScene::Init()
{
    m_titleLabel.SetFontType(FontType::TITLE_BIG);
    m_titleLabel.SetText(L"º¸½º¸¦ ¼±ÅÃÇÏ¼¼¿ä");
    m_titleLabel.SetSize({ 800.f, 140.f });
    m_titleLabel.SetPos({ WINDOW_WIDTH / 2.f, 120.f });

    float btnW = 100.f;
    float btnH = 100.f;
    float startX = 500.f;
    float gap = 50.f;

    m_btnBoss1 = UIButton(L"Boss1", []()
        {
            //Boss1 ¾À
            GET_SINGLE(SceneManager)->LoadScene(L"TestBossScene");
        });
    m_btnBoss1.SetSize({ btnW, btnH });
    m_btnBoss1.SetPos({ startX, WINDOW_HEIGHT / 2.f });

    m_btnBoss2 = UIButton(L"Boss2", []()
        {
            //Boss2 ¾À
            //GET_SINGLE(SceneManager)->LoadScene(L"");
        });
    m_btnBoss2.SetSize({ btnW, btnH });
    m_btnBoss2.SetPos({ startX + (btnW + gap), WINDOW_HEIGHT / 2.f });

    m_btnBoss3 = UIButton(L"Boss3", []()
        {
            //Boss3 ¾À
            //GET_SINGLE(SceneManager)->LoadScene(L"");
        });
    m_btnBoss3.SetSize({ btnW, btnH });
    m_btnBoss3.SetPos({ startX + 2 * (btnW + gap), WINDOW_HEIGHT / 2.f });

    m_btnTitle = UIButton(L"Å¸ÀÌÆ²·Î", []()
        {
            GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");
        });
    m_btnTitle.SetSize({ 800.f, 70.f });
    m_btnTitle.SetPos({ WINDOW_WIDTH / 2.f, startX + 50.f });
}

void BossSelectScene::Update()
{
    m_btnBoss1.Update();
    m_btnBoss2.Update();
    m_btnBoss3.Update();
    m_btnTitle.Update();
}

void BossSelectScene::Render(HDC hdc)
{
    m_titleLabel.Render(hdc);
    m_btnBoss1.Render(hdc);
    m_btnBoss2.Render(hdc);
    m_btnBoss3.Render(hdc);
    m_btnTitle.Render(hdc);
}
