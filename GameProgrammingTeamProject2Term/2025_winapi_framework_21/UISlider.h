#pragma once
#include "UIObject.h"

class UISlider : public UIObject
{
private:
    float m_value;
    bool m_dragging;

public:
    UISlider();
    virtual ~UISlider() {}

    float GetValue() const;
    void SetValue(float v);

    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};
