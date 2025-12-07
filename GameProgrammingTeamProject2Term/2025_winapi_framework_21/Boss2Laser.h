#pragma once
#include "Object.h"
#include "Texture.h"
#include "Animator.h"

class Collider;

// Boss2 수평 레이저 오브젝트
// - 화면 가로로 긴 막대 레이저
// - Boss2에서 위/아래에서 생성해서 중앙을 향해 Translate()로 이동시킴
class Boss2Laser : public Object
{
public:
    Boss2Laser();
    virtual ~Boss2Laser();

    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* other) override;

    // half-length(가로 반 길이)로 사용
    void SetArmLength(float len) { m_halfLength = len; UpdateColliderSize(); }
    void SetThickness(float t) { m_thickness = t;  UpdateColliderSize(); }

private:
    void UpdateColliderSize();
    void SetupAnimations();

private:
    Collider* m_collider = nullptr;
    Texture* m_pTexture = nullptr;
    Animator* m_animator = nullptr;

    float m_halfLength = 400.f; // 가로 반 길이
    float m_thickness = 40.f;  // 세로 두께
};
