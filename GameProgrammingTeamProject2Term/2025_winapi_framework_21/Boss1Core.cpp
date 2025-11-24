#include "pch.h"
#include "Boss1Core.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Boss1.h"
#include "EventBus.h"

Boss1Core::Boss1Core(Boss1* owner)
    : m_owner(owner)
{
    //Texture 있어야 함
    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"BossCore");
    SetSize({ 100.f, 100.f });
}

Boss1Core::~Boss1Core()
{
}

void Boss1Core::Update()
{
    if (!m_pendingSceneChange)
        return;

    m_delay -= GET_SINGLE(TimeManager)->GetDT();

    if (m_delay <= 0.f)
    {
        GET_SINGLE(SceneManager)->LoadScene(L"BossSelectScene");

        m_pendingSceneChange = false;
    }
}

void Boss1Core::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());

        EventBus::Invoke(L"Boss1Killed");

        m_delay = 0.2f;
        m_pendingSceneChange = true;
    }
}