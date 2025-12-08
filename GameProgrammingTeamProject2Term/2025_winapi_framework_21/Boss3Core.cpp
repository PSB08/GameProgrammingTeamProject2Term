#include "pch.h"
#include "Boss3Core.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Boss3.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Animation.h"

Boss3Core::Boss3Core(Boss3* owner)
    : m_owner(owner)
    , m_boss3Core(nullptr)
    , m_animator(nullptr)
{
    m_boss3Core = GET_SINGLE(ResourceManager)->GetTexture(L"boss3Core");

    //Texture 있어야 함
    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"BossCore");
    SetSize({ 1.f, 1.f });

    m_animator = AddComponent<Animator>();
    if (m_boss3Core && m_animator)
    {
        m_animator->CreateAnimation(
            L"bossCore3Loop",
            m_boss3Core,
            { 0.f, 0.f },
            { 128.f, 256.f },
            { 128.f, 0.f },
            12, 0.1f);
    }
    m_animator->Play(L"bossCore3Loop");
}

Boss3Core::~Boss3Core()
{
}

void Boss3Core::Update()
{
    if (!m_pendingSceneChange)
        return;

    m_delay -= GET_SINGLE(TimeManager)->GetDT();

    if (m_delay <= 0.f)
    {
        GET_SINGLE(SceneManager)->RequestLoadScene(L"ClearScene");

        m_pendingSceneChange = false;
    }
}

void Boss3Core::Render(HDC _hdc)
{
    ComponentRender(_hdc);
}

void Boss3Core::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        EventBus::Invoke(L"Boss3Killed");

        m_delay = 0.2f;
        m_pendingSceneChange = true;
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
        GET_SINGLE(ResourceManager)->Play(L"BossDie");
    }
}
