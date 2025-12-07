#include "pch.h"
#include "Boss2Laser.h"
#include "ResourceManager.h"
#include "Collider.h"

Boss2Laser::Boss2Laser()
    : m_collider(nullptr)
    , m_halfLength(400.f)
    , m_thickness(40.f)
{
    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetName(L"LaserLeft");

    UpdateColliderSize();
    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2Laser");

    m_animator = AddComponent<Animator>();
    if (m_pTexture && m_animator)
        SetupAnimations();
}

Boss2Laser::~Boss2Laser()
{
}

void Boss2Laser::Update()
{
}

void Boss2Laser::Render(HDC _hdc)
{
    if (m_animator && m_pTexture)
    {
        ComponentRender(_hdc);
        return;
    }
    else
    {
        Vec2 pos = GetPos();
        Vec2 size = GetSize();

        LONG left = (LONG)(pos.x - size.x * 0.5f);
        LONG right = (LONG)(pos.x + size.x * 0.5f);
        LONG top = (LONG)(pos.y - size.y * 0.5f);
        LONG bottom = (LONG)(pos.y + size.y * 0.5f);

        RECT rc = { left, top, right, bottom };

        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);
        HPEN   oldPen = (HPEN)SelectObject(_hdc, GetStockObject(NULL_PEN));

        FillRect(_hdc, &rc, brush);

        SelectObject(_hdc, oldPen);
        SelectObject(_hdc, oldBrush);
        DeleteObject(brush);

        ComponentRender(_hdc);
    }
}

void Boss2Laser::EnterCollision(Collider* other)
{
}

void Boss2Laser::UpdateColliderSize()
{
    Vec2 size;
    size.x = m_halfLength * 2.f;
    size.y = m_thickness;

    SetSize(size);

    if (m_collider)
        m_collider->SetSize(size);
}

void Boss2Laser::SetupAnimations()
{
    int texW = m_pTexture->GetWidth();
    int texH = m_pTexture->GetHeight();

    const int frameCount = 6;

    Vec2 sliceSize = { texW / (float)frameCount, (float)texH };
    Vec2 step = { sliceSize.x, 0.f };

    m_animator->CreateAnimation(
        L"boss2LaserLoop",
        m_pTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.08f
    );

    m_animator->Play(L"boss2LaserLoop", PlayMode::Loop, 1, 1.f);
}