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
    if (!m_interactable) return;

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
            m_onClick();  // 전달된 함수 실행
    }

    m_prevDown = nowDown;
}

void UIButton::Render(HDC hdc)
{
    RECT r = GetRect();

    Texture* bgTex = m_bgTexNormal;
    if (m_interactable && m_hover && m_bgTexHover)
        bgTex = m_bgTexHover;

    if (bgTex)
    {
        int dstW = r.right - r.left;
        int dstH = r.bottom - r.top;

        HDC memDC = bgTex->GetTextureDC();
        int srcW = bgTex->GetWidth();
        int srcH = bgTex->GetHeight();

        ::TransparentBlt(
            hdc,
            r.left, r.top, dstW, dstH,
            memDC,
            0, 0, srcW, srcH,
            RGB(0, 0, 0)
        );
    }
    else
    {
        HBRUSH brush = CreateSolidBrush(m_interactable ? RGB(255, 255, 255) : RGB(150, 150, 150));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

        HPEN hPen = ::CreatePen(PS_SOLID, 2, m_hover ? RGB(255, 0, 0) : RGB(0, 0, 0));
        HGDIOBJ oldPen = ::SelectObject(hdc, hPen);

        ::Rectangle(hdc, r.left, r.top, r.right, r.bottom);

        ::SelectObject(hdc, oldBrush);
        ::SelectObject(hdc, oldPen);

        ::DeleteObject(brush);
        ::DeleteObject(hPen);
    }

    if (m_texture)
    {
        int dstW = r.right - r.left;
        int dstH = r.bottom - r.top;

        HDC memDC = m_texture->GetTextureDC();
        int srcW = m_texture->GetWidth();
        int srcH = m_texture->GetHeight();

        TransparentBlt(
            hdc,
            r.left, r.top, dstW, dstH,
            memDC,
            0, 0, srcW, srcH,
            RGB(0, 0, 0)
        );

        return;
    }

    Texture* savedTex = m_texture;
    m_texture = nullptr;
    UIObject::Render(hdc);
    m_texture = savedTex;
}
