#pragma once
#include "Object.h"
#include "Texture.h"
#include "Animator.h"
#include "Collider.h"

class Boss2Shield : public Object
{
public:
    Boss2Shield(
        Object* owner,
        const std::wstring& idleTexName,
        const std::wstring& breakTexName,
        Vec2 shieldSize);
    virtual ~Boss2Shield();

    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* other) override;

    void StartBreak();
    bool IsBreaking() const { return m_isBreaking; }
    bool IsBroken()  const { return m_isBroken; }

private:
    void SetupAnimations();

private:
    Object* m_owner = nullptr;
    Texture* m_idleTexture = nullptr;
    Texture* m_breakTexture = nullptr;
    Animator* m_animator = nullptr;
    Collider* m_collider = nullptr;

    std::wstring  m_idleAnimName;
    std::wstring  m_breakAnimName;

    bool m_isBreaking = false;
    bool m_isBroken = false;
    float m_shieldSize;
};

