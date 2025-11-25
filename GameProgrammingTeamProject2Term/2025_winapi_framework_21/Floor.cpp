#include "pch.h"
#include "Floor.h"
#include "Collider.h"
Floor::Floor()
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 1000.f, 50.f });
	col->SetName(L"Floor");
}

Floor::~Floor()
{
}

void Floor::Update()
{
}

void Floor::Render(HDC _hdc)
{
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
    HPEN oldPen = (HPEN)SelectObject(_hdc, pen);

    Vec2 pos = GetPos();
    Collider* col = GetComponent<Collider>();
    Vec2 size = col->GetSize();

    Rectangle
    (   _hdc,
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
