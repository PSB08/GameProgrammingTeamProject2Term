#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "UIObject.h"
#include "CollisionManager.h"
#include "Rigidbody.h"

void Scene::FixedUpdate(float _fixedDT)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
		{
			if (auto rb = obj->GetComponent<Rigidbody>())
			{
				if (rb != nullptr)
					rb->FixedUpdate(_fixedDT);
			}
		}
	}
}
Scene::Scene()
{
}

Scene::~Scene()
{
	Release();
}

void Scene::Update()
{
	// ?? OBJ UPDATE
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if(!obj->GetIsDead())
				obj->Update();
	}
}
void Scene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if (!obj->GetIsDead())
				obj->LateUpdate();
	}
}

void Scene::Render(HDC _hdc)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			if (!obj->GetIsDead())
				obj->Render(_hdc);
	}

	// 2) UI 렌더 (맨 위에)
	for (auto* ui : m_vecUI)
	{
		ui->Render(_hdc);
	}
}

void Scene::Release()
{
	FlushEvent();
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& vec = m_vecObj[i];
		for (auto* obj : vec)
			SAFE_DELETE(obj);
		vec.clear();
	}

	for (auto* ui : m_vecUI)
		SAFE_DELETE(ui);
	m_vecUI.clear();

	for (auto* ui : m_killUI)
		SAFE_DELETE(ui);
	m_killUI.clear();

	GET_SINGLE(CollisionManager)->CheckReset();
}

void Scene::RequestDestroy(Object* obj)
{
	if (obj == nullptr)
		return;

	// 죽지 않으면 넣어라
	if (!obj->GetIsDead())
	{
		obj->SetDead();
		// 실제 delete는 FlushObject에서
		m_killObject.push_back(obj);
	}
}
void Scene::RequestDestroyUI(UIObject* obj)
{
	if (obj == nullptr)
		return;

	m_killUI.push_back(obj);
}

void Scene::FlushEvent()
{
	// 삭제
	for (Object* d : m_killObject)
	{
		RemoveObject(d);
		SAFE_DELETE(d);
	}
	m_killObject.clear();

	for (UIObject* u : m_killUI)
	{
		// m_vecUI에서 제거
		m_vecUI.erase(std::remove(m_vecUI.begin(), m_vecUI.end(), u), m_vecUI.end());
		SAFE_DELETE(u);
	}
	m_killUI.clear();

	// 2) 생성 처리
	for (auto& s : m_spawnObject)
	{
		AddObject(s.obj, s.type);
	}
	m_spawnObject.clear();
}

void Scene::RequestSpawn(Object* obj, Layer _type)
{
	if (!obj) return;
	m_spawnObject.push_back({ obj, _type });
}


void Scene::RemoveObject(Object* _obj)
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		auto& v = m_vecObj[i];
		v.erase(std::remove(v.begin(), v.end(), _obj), v.end());
	}

}