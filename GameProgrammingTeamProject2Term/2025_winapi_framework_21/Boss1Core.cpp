#include "pch.h"
#include "Boss1Core.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Boss1.h"

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

void Boss1Core::EnterCollision(Collider* _other)
{
    if (_other->IsTrigger())
    {
        if (_other->GetName() == L"PlayerBullet")
        {
            GET_SINGLE(SceneManager)->RequestDestroy(m_owner);
            GET_SINGLE(SceneManager)->RequestDestroy(this);
            GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
        }
    }
}
