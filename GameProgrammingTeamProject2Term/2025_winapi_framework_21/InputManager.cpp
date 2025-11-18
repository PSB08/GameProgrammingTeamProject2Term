#include "pch.h"
#include "InputManager.h"
#include "Core.h"
void InputManager::Init()
{
	m_vecKey.resize((int)KEY_TYPE::LAST, KeyInfo{ KEY_STATE::NONE, false });
	m_mousePos = {};
	m_mouseDelta = {};
	m_prevMousePos = m_mousePos;

	for (int i = 0; i < (int)KEY_TYPE::LAST; ++i)
	{
		assert(m_vkKey[i] != 0 && "KEY_TYPE, vkKey 매핑 에러");
	}
}


void InputManager::Update()
{
	HWND hWnd = ::GetFocus();
	//::GetActiveWindow();
	//::SetActiveWindow();
	if (hWnd == nullptr)
	{
		ResetAll();
		return;
	}

	UpdateKeys();
	UpdateMouse();
}

void InputManager::ResetAll()
{
	for (auto& e : m_vecKey)
	{
		e.isPrevCheck = false;
		e.state = KEY_STATE::NONE;
	}

	// mouse
	::GetCursorPos(&m_mousePos);
	::ScreenToClient(GET_SINGLE(Core)->GetHwnd(), &m_mousePos);
	m_prevMousePos = m_mousePos;
	m_mouseDelta = { 0,0 };
}


void InputManager::UpdateMouse()
{
	::GetCursorPos(&m_mousePos);

	::ScreenToClient(GET_SINGLE(Core)->GetHwnd(), &m_mousePos);

	m_mouseDelta.x = m_mousePos.x - m_prevMousePos.x;
	m_mouseDelta.y = m_mousePos.y - m_prevMousePos.y;
	m_prevMousePos = m_mousePos;
}



void InputManager::UpdateKeys()
{
	for (int i = 0; i < (int)KEY_TYPE::LAST; i++)
	{
		const bool pressed = (::GetAsyncKeyState(m_vkKey[i]) & 0x8000) != 0;
		auto& key = m_vecKey[i];

		if (pressed)
		{
			key.state = key.isPrevCheck ? KEY_STATE::PRESS : KEY_STATE::DOWN;
			key.isPrevCheck = true;
		}
		else
		{
			key.state = key.isPrevCheck ? KEY_STATE::UP : KEY_STATE::NONE;
			key.isPrevCheck = false;
		}
	}
}

