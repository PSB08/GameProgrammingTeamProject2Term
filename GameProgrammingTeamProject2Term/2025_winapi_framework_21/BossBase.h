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
        , m_bossActive(true)
    {
        SetSize({ 150.f, 150.f });
    }

    virtual ~BossBase() {}

    void Update() override
    {
        if (!m_bossActive)
            return;

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
        UpdatePattern();
    }

    void Render(HDC _hdc) override
    {
        if (!m_bossActive)
            return;

        ComponentRender(_hdc);
        RenderPattern(_hdc);
    }

    void SetActiveBoss(bool active) { m_bossActive = active; }
    bool IsActiveBoss() const { return m_bossActive; }

protected:
    virtual void StartRandomPattern() = 0;
    virtual void UpdatePattern() = 0;
    virtual void RenderPattern(HDC _hdc) {}

protected:
    Texture* m_pTexture;
    PatternEnum m_curPattern;

    float m_patternTimer;
    float m_cooldownTimer;

    bool m_isCooldown;
    float m_cooldownDuration = 3.f;

    int m_patternCount = 0;
    const int m_maxPatternCount = 1;

    bool m_bossActive;
};
