#include "pch.h"
#include "Boss2Core.h"
#include "Boss2.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "Texture.h"

Boss2Core::Boss2Core(Boss2* owner, int index)
    : m_owner(owner)
    , m_index(index)
    , m_isOpened(false)
    , m_isDead(false)
    , m_collidable(false)
    , m_hp(1)
    , m_pTexture(nullptr)
    , m_collider(nullptr)
{
    SetSize({ 60.f, 60.f });

    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"plane");

    m_collider = AddComponent<Collider>();
    m_collider->SetTrigger(true);
    m_collider->SetSize(GetSize());
    m_collider->SetName(L"Boss2Core");
    m_collider->SetEnabled(false);
}

Boss2Core::~Boss2Core()
{
}

void Boss2Core::Update()
{
    if (m_isDead)
        return;

    if (m_isOpened && m_hp <= 0)
    {
        HandleDeath();
    }
}

void Boss2Core::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();

    RECT rc =
    {
        (LONG)(pos.x - size.x / 2.f),
        (LONG)(pos.y - size.y / 2.f),
        (LONG)(pos.x + size.x / 2.f),
        (LONG)(pos.y + size.y / 2.f)
    };

    if (m_isOpened)
    {
        HBRUSH b = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(_hdc, &rc, b);
        DeleteObject(b);
    }
    else
    {
        HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, brush);

        HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
        HPEN oldPen = (HPEN)SelectObject(_hdc, pen);

        Rectangle(_hdc, rc.left, rc.top, rc.right, rc.bottom);

        SelectObject(_hdc, oldPen);
        DeleteObject(pen);
        SelectObject(_hdc, oldBrush);
    }

    ComponentRender(_hdc);
}

void Boss2Core::EnterCollision(Collider* _other)
{
    if (m_isDead)
        return;

    if (!m_isOpened)
        return;

    if (!m_collidable)
        return;

    if (_other->GetName() == L"PlayerBullet")
    {
        m_hp -= 100;
    }
}

void Boss2Core::OpenCore()
{
    m_isOpened = true;
    m_collidable = true;
    m_collider->SetEnabled(true);
}

void Boss2Core::HandleDeath()
{
    if (m_isDead)
        return;

    m_isDead = true;
    SetDead();

    if (m_owner)
        m_owner->NotifyCoreDestroyed(this);
}
