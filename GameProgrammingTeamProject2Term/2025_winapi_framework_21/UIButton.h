#pragma once
#include "pch.h"
#include "UIObject.h"
#include <functional>

using std::function;  //function은 호출할 수 있는 객체를 보관가능하고 호출 가능한 타입.

class UIButton : public UIObject
{
private:
    function<void()> m_onClick = nullptr; //버튼이 클릭되면 실행할 함수를 저장해 두는 콜백 역할
    //람다랑 함수 포인터를 쓸 수 있음

    bool m_prevDown = false;
    bool m_hover = false;

public:
    UIButton() = default;

    UIButton(const wstring& text, function<void()> cb)
        : m_onClick(cb)
    {
        m_text = text;  //버튼을 생성할 때 눌렀을 때 실행될 함수를 전달
    }

    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};
