#include "pch.h"
#include "LaserObject.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"

LaserObject::LaserObject(bool isLeftLaser)
    : m_isLeft(isLeftLaser)
{
    SetSize({ 50.f, (float)WINDOW_HEIGHT });

    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetSize({ 50.f, (float)WINDOW_HEIGHT });

    if (isLeftLaser)
        col->SetName(L"LaserLeft");
    else
        col->SetName(L"LaserRight");
}

LaserObject::~LaserObject()
{
}

void LaserObject::Update()
{
    Translate({ (float)m_dir * m_speed * fDT, 0.f });

    if (m_isLeft && GetPos().x > WINDOW_WIDTH / 2.f)
    {
        GET_SINGLE(SceneManager)->RequestDestroy(this);
    }
    else if (!m_isLeft && GetPos().x < WINDOW_WIDTH / 2.f)
    {
        GET_SINGLE(SceneManager)->RequestDestroy(this);
    }
}

void LaserObject::Render(HDC _hdc)
{
    RECT rc =
    {
        (LONG)GetPos().x,
        0,
        (LONG)(GetPos().x + GetSize().x),
        (LONG)GetSize().y
    };

    HBRUSH b = CreateSolidBrush(RGB(255, 255, 0));
    FillRect(_hdc, &rc, b);
    DeleteObject(b);
}

void LaserObject::EnterCollision(Collider* _other)
{
    cout << "EnterCollision" << endl;
}
