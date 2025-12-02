#pragma once
#include "Scene.h"
#include "UIButton.h"
#include "Texture.h"
#include "EventBus.h"
#include <memory>
#include <iostream>

class BossSelectScene : public Scene
{
public:
	BossSelectScene();
	~BossSelectScene();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override {}
    virtual void FixedUpdate(float) override {}
    virtual void Render(HDC hdc) override;
    virtual void Release() override {}
    
    void EventListener();

private:
    UIObject m_titleLabel;
    UIButton m_btnBoss1;
    UIButton m_btnBoss2;
    UIButton m_btnBoss3;
    UIButton m_btnTitle;

    Texture* m_btn1Texture = nullptr;
    Texture* m_btn2Texture = nullptr;
    Texture* m_btn3Texture = nullptr;

private:
    EventBus::ListenerID m_boss1Listener;
    EventBus::ListenerID m_boss2Listener;
    EventBus::ListenerID m_boss3Listener;
};

