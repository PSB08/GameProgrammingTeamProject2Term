#pragma once
#include "pch.h"
#include "Texture.h"

class UIObject
{
public:
    Vec2 m_pos;
    Vec2 m_size;
    wstring m_text;
    Texture* m_texture = nullptr;
    FontType m_fontType = FontType::TITLE;

public:
    virtual ~UIObject() {}

    RECT GetRect() const
    {
        int left = (int)std::lround(m_pos.x - m_size.x * 0.5f);
        int top = (int)std::lround(m_pos.y - m_size.y * 0.5f);
        int right = (int)std::lround(m_pos.x + m_size.x * 0.5f);
        int bottom = (int)std::lround(m_pos.y + m_size.y * 0.5f);

        return RECT{ left, top, right, bottom };
    }

    virtual void Update() {}

    virtual void Render(HDC hdc)
    {
        RECT r = GetRect();

        GDISelector selFont(hdc, m_fontType);

        int oldBk = SetBkMode(hdc, TRANSPARENT);
        DrawTextW(
            hdc,
            m_text.c_str(),
            (int)m_text.size(),
            &r,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE
        );
        SetBkMode(hdc, oldBk);
    }

    void SetFontType(FontType f) { m_fontType = f; }
    void SetPos(Vec2 v) { m_pos = v; }
    void SetSize(Vec2 v) { m_size = v; }
    void SetText(const wstring& t) { m_text = t; }
};
