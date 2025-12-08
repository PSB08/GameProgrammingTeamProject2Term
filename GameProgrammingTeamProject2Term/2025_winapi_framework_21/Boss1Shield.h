#pragma once
#include "Object.h"
#include "Texture.h"
#include "Animator.h"
#include "Collider.h"

class Boss1;

class Boss1Shield : public Object
{
public:
	Boss1Shield(Boss1* owner,
        const std::wstring& idleTexName,
        const std::wstring& breakTexName,
        Vec2 colliderSize);
	~Boss1Shield();

    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;

    void StartBreak();

    bool IsBreaking() const { return m_isBreaking; }

private:
    void SetupAnimations();

private:
    Boss1* m_owner = nullptr;
    Texture* m_idleTexture = nullptr;
    Texture* m_breakTexture = nullptr;
    Animator* m_animator = nullptr;
    Collider* m_collider = nullptr;

    bool m_isBreaking = false;
    float shieldSize;

    std::wstring m_idleAnimName;
    std::wstring m_breakAnimName;
};

