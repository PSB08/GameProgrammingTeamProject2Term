#include "pch.h"
#include "UIButton.h"
#include <Windows.h>

// 공용 함수
static bool IsInside(const POINT& p, const RECT& r)
{
    return p.x >= r.left && p.x <= r.right &&
        p.y >= r.top && p.y <= r.bottom;
}

void UIButton::Update()
{
    RECT rect = GetRect();

    POINT pt;
    ::GetCursorPos(&pt);

    HWND hwnd = ::GetActiveWindow();
    if (hwnd)
        ::ScreenToClient(hwnd, &pt);

    m_hover = IsInside(pt, rect);

    bool nowDown = (::GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

    if (!nowDown && m_prevDown && m_hover)
    {
        if (m_onClick)
            m_onClick();
    }

    m_prevDown = nowDown;
}

void UIButton::Render(HDC hdc)
{
    RECT r = GetRect();

    HPEN hPen = ::CreatePen(PS_SOLID, 2, m_hover ? RGB(255, 0, 0) : RGB(0, 0, 0));
    HGDIOBJ oldPen = ::SelectObject(hdc, hPen);

    HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));

    ::Rectangle(hdc, r.left, r.top, r.right, r.bottom);

    ::SelectObject(hdc, oldBrush);
    ::SelectObject(hdc, oldPen);
    ::DeleteObject(hPen);

    UIObject::Render(hdc);
}
