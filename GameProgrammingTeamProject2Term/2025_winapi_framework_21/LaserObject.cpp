#include "pch.h"
#include "LaserObject.h"
#include "Collider.h"
#include "SceneManager.h"
#include "ResourceManager.h"
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

    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"LaserVertical");

    m_animator = AddComponent<Animator>();
    if (m_pTexture && m_animator)
        SetupAnimations();
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
    if (m_animator && m_pTexture)
    {
        ComponentRender(_hdc);
        return;
    }
    else
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

        ComponentRender(_hdc);
    }
}

void LaserObject::EnterCollision(Collider* _other)
{
}

void LaserObject::SetupAnimations()
{
    int texW = m_pTexture->GetWidth();
    int texH = m_pTexture->GetHeight();

    const int frameCount = 16;

    Vec2 sliceSize = { texW / (float)frameCount, (float)texH };
    Vec2 step = { sliceSize.x, 0.f };

    m_animator->CreateAnimation(
        L"laserLoop",
        m_pTexture,
        { 0.f, 0.f },
        sliceSize,
        step,
        frameCount,
        0.05f
    );

    m_animator->Play(L"laserLoop", PlayMode::Loop, 1, 1.f);
}