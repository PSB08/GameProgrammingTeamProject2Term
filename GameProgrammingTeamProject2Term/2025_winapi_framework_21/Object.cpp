#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "Component.h"
Object::Object()
	: m_pos{}
	, m_size{}
	, m_isDie(false)
{

}

Object::~Object()
{
	for (Component* com : m_vecComponents)
		SAFE_DELETE(com);
	m_vecComponents.clear();
}

void Object::Update()
{
}

void Object::LateUpdate()
{
	for (Component* com : m_vecComponents)
	{
		if (com != nullptr)
			com->LateUpdate();
	}

}

void Object::Render(HDC _hdc)
{
	RECT_RENDER(_hdc, m_pos.x, m_pos.y
				, m_size.x, m_size.y);
}

void Object::ComponentRender(HDC _hdc)
{
	for (Component* com : m_vecComponents)
	{
		if (com != nullptr)
			com->Render(_hdc);
	}
}
