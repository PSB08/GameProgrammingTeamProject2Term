#pragma once
#include "pch.h"
#include "UIObject.h"
#include <functional>

using std::function;  //function은 호출할 수 있는 객체를 보관가능하고 호출 가능한 타입.

class UIButton : public UIObject
{
private:
    function<void()> m_onClick = nullptr;

    bool m_prevDown = false;
    bool m_hover = false;

public:
    UIButton() = default;

    UIButton(const wstring& text, function<void()> cb)
        : m_onClick(cb)
    {
        m_text = text;
    }

    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};
