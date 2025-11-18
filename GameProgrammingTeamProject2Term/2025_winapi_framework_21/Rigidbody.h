#pragma once
#include "Component.h"
class Rigidbody :
    public Component

{
public:
    Rigidbody();
    ~Rigidbody();
public:
    // Component을(를) 통해 상속됨
    void Init() override;
    void LateUpdate() override;
    void FixedUpdate(float _fixedDT); // 물리 계산
    void Render(HDC hDC) override;
public:
    void SetMass(float _mass) { m_mass = _mass; }
    void AddForce(Vec2 _force) { m_force += _force; }

    // 순간적인 힘 (점프, 피격 넉백, 폭발)
    void AddImpulse(Vec2 _impulse)
    {
        //m_velocity += _impulse / m_mass; // 이거하면 getmass해서 곱해줘야함.
        m_velocity += _impulse;
    }
    void SetGrounded(bool _ground) { m_grounded = _ground; }
    bool IsGrounded() const { return m_grounded; }

    void SetUseGravity(bool _useGravity) { m_useGravity = _useGravity; }

    void SetIsKinematic(bool _isKinematic) { m_isKinematic = _isKinematic; }
    bool IsKinematic() const { return m_isKinematic; }

    void SetVelocity(Vec2 _velocity) { m_velocity = _velocity; }
    const Vec2& GetVelocity() const { return m_velocity; }

    void SetFriction(float _friction) { m_friction = _friction; }
private:
    bool m_grounded;
    bool m_useGravity;
    bool m_isKinematic;

    float m_mass; // 질량
    float m_friction; // 마찰력
    float m_airDrag;  // 공중 감속 (Drag)

    Vec2  m_force; // 힘
    Vec2 m_velocity; // 속도
    Vec2 m_maxVelocity; // 제한속도
    Vec2 m_gravity; // 중력
    Vec2 m_accelation; // 가속도
};
