#include "pch.h"
#include "BossSelectScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "EventBus.h"
#include "GameData.h"
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
    EventBus::RemoveListener(L"Boss1Killed", m_boss1Listener);
    EventBus::RemoveListener(L"Boss2Killed", m_boss2Listener);
    EventBus::RemoveListener(L"Boss3Killed", m_boss3Listener);
}

void BossSelectScene::Init()
{
    EventListener();
    GET_SINGLE(ResourceManager)->Play(L"BOSSSELECT");
    Texture* backNormal = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Normal");
    Texture* backHover = GET_SINGLE(ResourceManager)->GetTexture(L"BackBtn_Hover");

    m_titleLabel.SetFontType(FontType::TITLE_BIG);
    m_titleLabel.SetFontColor(RGB(255, 255, 255));
    m_titleLabel.SetText(L"º¸½º¸¦ ¼±ÅÃÇÏ¼¼¿ä");
    m_titleLabel.SetSize({ 800.f, 140.f });
    m_titleLabel.SetPos({ WINDOW_WIDTH / 2.f, 120.f });

    float btnW = 150.f;
    float btnH = 150.f;
    float startX = 450.f;
    float startY = 525.f;
    float gap = 50.f;

    m_btn1Texture = GET_SINGLE(ResourceManager)->GetTexture(L"boss1Icon");
    m_btn2Texture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2Icon");

    m_btnBoss1 = UIButton(m_btn1Texture, []()
        {
            //Boss1 ¾À
            GET_SINGLE(ResourceManager)->StopAllSounds();
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"BossScene");
        });
    m_btnBoss1.SetSize({ btnW, btnH });
    m_btnBoss1.SetPos({ startX, WINDOW_HEIGHT / 2.f });

    m_btnBoss2 = UIButton(m_btn2Texture, []()
        {
            //Boss2 ¾À
            GET_SINGLE(ResourceManager)->StopAllSounds();
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"Boss2Scene");
        });
    m_btnBoss2.SetSize({ btnW, btnH });
    m_btnBoss2.SetPos({ startX + (btnW + gap), WINDOW_HEIGHT / 2.f });

    m_btnBoss3 = UIButton(L"Boss3", []()
        {
            //Boss3 ¾À
            GET_SINGLE(ResourceManager)->StopAllSounds();
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"Boss3Scene");
        });
    m_btnBoss3.SetSize({ btnW, btnH });
    m_btnBoss3.SetPos({ startX + 2 * (btnW + gap), WINDOW_HEIGHT / 2.f });

    m_btnTitle = UIButton(L"Å¸ÀÌÆ²·Î", []()
        {
            GET_SINGLE(ResourceManager)->Play(L"UIButton");
            GET_SINGLE(SceneManager)->RequestLoadScene(L"TitleScene");
        });
    m_btnTitle.SetSize({ 600.f, 70.f });
    m_btnTitle.SetPos({ startX + (btnW + gap), startY });

    auto data = GameData::GetInstance();
    m_btnBoss1.Interactable(!data->boss1Cleared);
    m_btnBoss2.Interactable(!data->boss2Cleared);
    m_btnBoss3.Interactable(!data->boss3Cleared);

    //m_btnBoss1.SetBackgroundTexture(backNormal, backHover);
    //m_btnBoss2.SetBackgroundTexture(backNormal, backHover);
    //m_btnBoss3.SetBackgroundTexture(backNormal, backHover);
    m_btnTitle.SetBackgroundTexture(backNormal, backHover);
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

void BossSelectScene::EventListener()
{
    m_boss1Listener = EventBus::AddListener(L"Boss1Killed", [this]()
        {
            GameData::GetInstance()->boss1Cleared = true;
            GameData::GetInstance()->boss2Cleared = false;
            GameData::GetInstance()->boss3Cleared = false;
        });

    m_boss2Listener = EventBus::AddListener(L"Boss2Killed", [this]()
        {
            GameData::GetInstance()->boss1Cleared = false;
            GameData::GetInstance()->boss2Cleared = true;
            GameData::GetInstance()->boss3Cleared = false;
        });

    m_boss3Listener = EventBus::AddListener(L"Boss3Killed", [this]()
        {
            GameData::GetInstance()->boss1Cleared = false;
            GameData::GetInstance()->boss2Cleared = false;
            GameData::GetInstance()->boss3Cleared = true;
        });
}
