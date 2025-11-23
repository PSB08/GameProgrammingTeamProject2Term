#include "pch.h"
#include "UISlider.h"
#include "InputManager.h"

extern bool KEY_DOWN(int vk);
extern bool KEY_UP(int vk);

UISlider::UISlider()
    : m_value(1.f), m_dragging(false)
{
}

float UISlider::GetValue() const
{
    return m_value;
}

void UISlider::SetValue(float v)
{
    m_value = std::clamp(v, 0.f, 1.f);
}

void UISlider::Update()
{
    if (KEY_DOWN(VK_LBUTTON))
    {
        RECT rc = GetRect();
        POINT pt = GET_MOUSEPOS;

        if (PtInRect(&rc, pt))
            m_dragging = true;
    }
    else if (KEY_UP(VK_LBUTTON))
    {
        m_dragging = false;
    }

    if (m_dragging)
    {
        RECT rc = GetRect();

        float left = (float)rc.left;
        float width = (float)(rc.right - rc.left);

        float mx = (float)GET_MOUSEPOS.x;

        float ratio = (mx - left) / width;
        m_value = std::clamp(ratio, 0.f, 1.f);
    }
}

void UISlider::Render(HDC hdc)
{
    RECT rc = GetRect();

    HBRUSH bg = CreateSolidBrush(RGB(80, 80, 80));
    FillRect(hdc, &rc, bg);
    DeleteObject(bg);

    RECT fill = rc;
    fill.right = (int)(rc.left + (rc.right - rc.left) * m_value);

    HBRUSH fg = CreateSolidBrush(RGB(200, 200, 50));
    FillRect(hdc, &fill, fg);
    DeleteObject(fg);

    UIObject::Render(hdc);
}

bool KEY_DOWN(int vk)
{
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

bool KEY_UP(int vk)
{
    return !(GetAsyncKeyState(vk) & 0x8000);
}
