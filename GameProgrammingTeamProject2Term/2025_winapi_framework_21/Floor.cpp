#include "pch.h"
#include "Floor.h"
#include "Collider.h"
#include "ResourceManager.h"

Floor::Floor()
{
    auto* col = AddComponent<Collider>();

    col->SetSize({ m_renderSize.x, 20.f });
    col->SetOffSetPos(Vec2(0.f, (m_renderSize.y - 85.f) / 2.f));
    col->SetName(L"Floor");

    m_texFloor = GET_SINGLE(ResourceManager)->GetTexture(L"Ground");
}

Floor::~Floor()
{
}

void Floor::Update()
{
}

void Floor::Render(HDC _hdc)
{
    Vec2 pos = GetPos();

    Vec2 size = m_renderSize;

    int left = (int)(pos.x - size.x / 2);
    int top = (int)(pos.y - size.y / 2);
    int right = (int)(pos.x + size.x / 2);
    int bottom = (int)(pos.y + size.y / 2);

    int dstW = right - left;
    int dstH = bottom - top;

    if (m_texFloor)
    {
        int srcW = m_texFloor->GetWidth();
        int srcH = m_texFloor->GetHeight();

        ::TransparentBlt(
            _hdc,
            left, top, dstW, dstH,
            m_texFloor->GetTextureDC(),
            0, 0, srcW, srcH,
            RGB(0, 0, 0)
        );
    }
    else
    {
        HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);

        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
        HPEN oldPen = (HPEN)SelectObject(_hdc, pen);

        ::Rectangle(_hdc, left, top, right, bottom);

        SelectObject(_hdc, oldBrush);
        SelectObject(_hdc, oldPen);

        DeleteObject(brush);
        DeleteObject(pen);
    }

    ComponentRender(_hdc);
}