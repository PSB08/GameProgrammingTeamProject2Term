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
    , m_pBrokenTexture(nullptr)
    , m_collider(nullptr)
{
    SetSize({ 60.f, 60.f });

    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2Subcore");
    m_pBrokenTexture = GET_SINGLE(ResourceManager)->GetTexture(L"boss2SubcoreBreak");

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
    Texture* tex = nullptr;

    if (m_isDead)
        tex = m_pBrokenTexture ? m_pBrokenTexture : m_pTexture;
    else
        tex = m_pTexture;

    if (!tex)
        return;

    Vec2 pos = GetPos();
    Vec2 size = GetSize();

    int texW = tex->GetWidth();
    int texH = tex->GetHeight();

    int dx = (int)(pos.x - size.x / 2.f);
    int dy = (int)(pos.y - size.y / 2.f);
    int dw = (int)size.x;
    int dh = (int)size.y;

    int sx = 0;
    int sy = 0;
    int sw = texW;
    int sh = texH;

    ::TransparentBlt(_hdc,
        dx, dy, dw, dh,
        tex->GetTextureDC(),
        sx, sy, sw, sh,
        RGB(0, 0, 0));

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
