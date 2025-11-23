#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "TestBossScene.h"
#include "TitleScene.h"
#include "SettingScene.h"
#include "Object.h"
#include "Collider.h"

void SceneManager::PhysicsSyncColliders()
{
	for (UINT i = 0; i < (UINT)Layer::END; ++i)
	{
		const auto& objects = m_curScene->GetLayerObjects((Layer)i);
		for (Object* obj : objects)
		{
			if (!obj)
				continue;

			if (auto* col = obj->GetComponent<Collider>())
				col->LateUpdate(); // sync  
		}
	}
}

void SceneManager::FixedUpdate(float _fixedDT)
{
	if (m_curScene == nullptr || m_curSceneStop)
		return;
	m_curScene->FixedUpdate(_fixedDT);
	PhysicsSyncColliders();
}

void SceneManager::Init()
{

	m_curSceneStop = false;
	m_curScene = nullptr;
	// ¾À µî·Ï
	RegisterScene(L"DevScene",std::make_shared<DevScene>());
	RegisterScene(L"TestBossScene",std::make_shared<TestBossScene>());
	RegisterScene(L"TitleScene",std::make_shared<TitleScene>());
	RegisterScene(L"SettingScene",std::make_shared<SettingScene>());
	RegisterScene(L"DeadScene",std::make_shared<SettingScene>());
	
	// Ã¹ ¾À ·Îµå
	LoadScene(L"TitleScene");
}

void SceneManager::Update()
{
	if (m_curScene == nullptr || m_curSceneStop)
		return;
	m_curScene->Update();
	m_curScene->LateUpdate();
}


void SceneManager::Render(HDC _hdc)
{
	if (m_curScene == nullptr) // Render´Â ¸ØÃç¹ö¸®¸é ±×¸®Áö ¸øÇÏ±â¿¡ ³ÀµÒ
		return;
	m_curScene->Render(_hdc);
}



void SceneManager::RegisterScene(const wstring& _name, std::shared_ptr<Scene> _scene)
{
	if (_name.empty() || _scene == nullptr)
		return;
	m_mapScenes.insert(m_mapScenes.end(), {_name, _scene});
}

void SceneManager::LoadScene(const wstring& _name)
{
	// ¾ÀÀÌ ÀÌ¹Ì ·ÎµåµÇ¾îÀÖ´Ù´Â°Í
	if (m_curScene != nullptr)
	{
		m_curScene->Release();
		m_curScene = nullptr;
	}

	auto iter = m_mapScenes.find(_name);
	if (iter != m_mapScenes.end())
	{
		//???
		m_curScene = iter->second;
		m_curScene->Init();
	}
}
