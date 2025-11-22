#include "pch.h"
#include "Boss3Core.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Boss3.h"

Boss3Core::Boss3Core(Boss3* owner)
    : m_owner(owner)
{
    //Texture 있어야 함
    Collider* col = AddComponent<Collider>();
    col->SetTrigger(true);
    col->SetName(L"BossCore");
    SetSize({ 100.f, 100.f });
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
        GET_SINGLE(SceneManager)->LoadScene(L"TitleScene");

        m_pendingSceneChange = false;
    }
}

void Boss3Core::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger() && _other->GetName() == L"PlayerBullet")
    {
        GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());

        m_delay = 0.2f;
        m_pendingSceneChange = true;
    }
}
