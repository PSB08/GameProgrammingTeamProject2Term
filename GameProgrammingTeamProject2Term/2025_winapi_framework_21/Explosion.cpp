#include "pch.h"
#include "Explosion.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Animator.h"
#include "ResourceManager.h"

Explosion::Explosion()
	: m_explosingTime(1.1f)
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 450.f, 450.f });
	col->SetName(L"Explosion");
    GET_SINGLE(SceneManager)->GetCurScene()->StartShake(0.6f, 150.f);
    auto* ani = AddComponent<Animator>();

    m_ExpTexture = GET_SINGLE(ResourceManager)->GetTexture(L"VerybigExplosion");

    ani->CreateAnimation
    (L"Explose"
        , m_ExpTexture
        , { 0.f,0.f }
        , { 480.f,480.f }
        , { 480.f,0.f }
    , 11, 0.1f);

    ani->Play(L"Explose", PlayMode::Once);
}

Explosion::~Explosion()
{
}

void Explosion::Update()
{
    if (m_timer < m_explosingTime)
        m_timer += fDT;
    else
        SetDead();
}

void Explosion::Render(HDC _hdc)
{
    //HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
    //HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);

    //HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
    //HPEN oldPen = (HPEN)SelectObject(_hdc, pen);

    //Vec2 pos = GetPos();
    //Collider* col = GetComponent<Collider>();
    //Vec2 size = col->GetSize();

    //Rectangle
    //(_hdc,
    //    (int)(pos.x - size.x / 2),
    //    (int)(pos.y - size.y / 2),
    //    (int)(pos.x + size.x / 2),
    //    (int)(pos.y + size.y / 2)
    //);

    //SelectObject(_hdc, oldBrush);
    //SelectObject(_hdc, oldPen);

    //DeleteObject(brush);
    //DeleteObject(pen);

    ComponentRender(_hdc);
}
