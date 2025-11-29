#include "pch.h"
#include "Explosion.h"
#include "Collider.h"

Explosion::Explosion()
	: m_explosingTime(0.5f)
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 500.f, 800.f });
	col->SetName(L"Explosion");
}

Explosion::~Explosion()
{
}

void Explosion::Update()
{
    if (m_timer < m_explosingTime)
        m_timer += fDT;
    else
        SetDead();
}

void Explosion::Render(HDC _hdc)
{
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
    HPEN oldPen = (HPEN)SelectObject(_hdc, pen);

    Vec2 pos = GetPos();
    Collider* col = GetComponent<Collider>();
    Vec2 size = col->GetSize();

    Rectangle
    (_hdc,
        (int)(pos.x - size.x / 2),
        (int)(pos.y - size.y / 2),
        (int)(pos.x + size.x / 2),
        (int)(pos.y + size.y / 2)
    );

    SelectObject(_hdc, oldBrush);
    SelectObject(_hdc, oldPen);

    DeleteObject(brush);
    DeleteObject(pen);

    ComponentRender(_hdc);
}
