#pragma once
#include "Object.h"
#include "Texture.h"

template <typename PatternEnum>
class BossBase : public Object
{
public:
    BossBase()
        : m_curPattern(PatternEnum::NONE)
        , m_patternTimer(0.f)
        , m_cooldownTimer(0.f)
        , m_isCooldown(false)
    {
        SetSize({ 150.f, 150.f });
    }

    virtual ~BossBase() {}

    void Update() override
    {
        if (m_isCooldown)
        {
            m_cooldownTimer += fDT;
            if (m_cooldownTimer >= m_cooldownDuration)
            {
                m_isCooldown = false;
                m_cooldownTimer = 0.f;
                StartRandomPattern();
            }
            return;
        }

        m_patternTimer += fDT;
        UpdatePattern();  // 자식이 구현
    }

    void Render(HDC _hdc) override
    {
        ComponentRender(_hdc);
        RenderPattern(_hdc); // 자식이 필요하면 구현
    }

protected:
    //보스가 구현해야 하는 항목
    virtual void StartRandomPattern() = 0;
    virtual void UpdatePattern() = 0;
    virtual void RenderPattern(HDC _hdc) {}

protected:
    Texture* m_pTexture;
    // 패턴
    PatternEnum m_curPattern;

    // 타이머
    float m_patternTimer;
    float m_cooldownTimer;

    bool m_isCooldown;
    float m_cooldownDuration = 3.f;

    //패턴 카운트
    int m_patternCount = 0;
    const int m_maxPatternCount = 10;
};
