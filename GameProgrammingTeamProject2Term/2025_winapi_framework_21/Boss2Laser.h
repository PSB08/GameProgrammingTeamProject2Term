#pragma once
#include "Object.h"

class Collider;

// 보스 중심에서 십자(4방향) 레이저를 그리는 오브젝트.
// - 내부에 각도(m_angle)를 가지고 계속 회전
// - Boss2 쪽에서는 생성 + 위치 SetPos 만 해주면 됨.
class Boss2Laser : public Object
{
public:
    Boss2Laser();
    virtual ~Boss2Laser();

    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* other) override;

    void SetRotateSpeed(float s) { m_rotateSpeed = s; }
    void SetArmLength(float len) { m_armLength = len; UpdateColliderSize(); }
    void SetThickness(float t) { m_thickness = t;  UpdateColliderSize(); }

private:
    void DrawBeam(HDC _hdc, float baseAngle); // 한 방향(양쪽) 빔 그리기
    void UpdateColliderSize();

private:
    Collider* m_collider;

    float m_angle;        // 현재 회전 각도 (라디안)
    float m_rotateSpeed;  // 회전 속도 (라디안/초)
    float m_armLength;    // 각 빔의 반 길이
    float m_thickness;    // 빔의 두께
};
