#include "pch.h"
#include "Boss2MainCore.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Boss1.h"
#include "EventBus.h"

Boss2MainCore::Boss2MainCore(Boss2* boss)
	: m_owner(boss)
{
    //Texture 있어야 함
    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"Boss2MainCore");
    SetSize({ 100.f, 100.f });
}

Boss2MainCore::~Boss2MainCore()
{
}

void Boss2MainCore::Update()
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

void Boss2MainCore::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());

        EventBus::Invoke(L"Boss2Killed");

        m_delay = 0.2f;
        m_pendingSceneChange = true;
    }
}
