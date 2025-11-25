#include "pch.h"
#include "Boss2Laser.h"
#include "Collider.h"
#include <cmath>

Boss2Laser::Boss2Laser()
    : m_collider(nullptr)
    , m_angle(0.f)
    , m_rotateSpeed(PI)
    , m_armLength(400.f)
    , m_thickness(40.f)
{
    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetName(L"LaserLeft");

    UpdateColliderSize();
}

Boss2Laser::~Boss2Laser()
{
}

void Boss2Laser::Update()
{
    m_angle += m_rotateSpeed * fDT;
    // 회전 자체는 Render 에서 반영.
}

void Boss2Laser::Render(HDC _hdc)
{
    DrawBeam(_hdc, m_angle);
    DrawBeam(_hdc, m_angle + 3.14159265f * 0.5f);

    ComponentRender(_hdc);
}

void Boss2Laser::EnterCollision(Collider* other)
{
}

// 한 방향(양쪽) 빔을 회전시켜서 그린다.
void Boss2Laser::DrawBeam(HDC _hdc, float baseAngle)
{
    Vec2 center = GetPos();

    float dx = std::cos(baseAngle);
    float dy = std::sin(baseAngle);

    // 빔의 두께 방향 단위 벡터 (수직)
    float px = -dy;
    float py = dx;

    float halfT = m_thickness * 0.5f;

    Vec2 p1, p2, p3, p4;

    p1.x = center.x + dx * m_armLength + px * halfT;
    p1.y = center.y + dy * m_armLength + py * halfT;

    p2.x = center.x + dx * m_armLength - px * halfT;
    p2.y = center.y + dy * m_armLength - py * halfT;

    p3.x = center.x - dx * m_armLength - px * halfT;
    p3.y = center.y - dy * m_armLength - py * halfT;

    p4.x = center.x - dx * m_armLength + px * halfT;
    p4.y = center.y - dy * m_armLength + py * halfT;

    POINT pts[4] =
    {
        { (LONG)p1.x, (LONG)p1.y },
        { (LONG)p2.x, (LONG)p2.y },
        { (LONG)p3.x, (LONG)p3.y },
        { (LONG)p4.x, (LONG)p4.y },
    };

    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);
    HPEN   oldPen = (HPEN)SelectObject(_hdc, GetStockObject(NULL_PEN));

    Polygon(_hdc, pts, 4);

    SelectObject(_hdc, oldPen);
    SelectObject(_hdc, oldBrush);
    DeleteObject(brush);
}

void Boss2Laser::UpdateColliderSize()
{
    // 레이저 전체를 대충 감싸는 정사각형 콜라이더
    float half = m_armLength + m_thickness;
    Vec2 size;
    size.x = half * 2.f;
    size.y = half * 2.f;

    SetSize(size);
    if (m_collider)
        m_collider->SetSize(size);
}
