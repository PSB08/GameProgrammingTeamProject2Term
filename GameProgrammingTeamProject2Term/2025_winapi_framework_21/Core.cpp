#include "pch.h"
#include "Core.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"

bool Core::Init(HWND _hWnd)
{
	srand((unsigned int)time(nullptr));

	m_hWnd = _hWnd;
	m_hDC = ::GetDC(m_hWnd);
	
	// 더블버퍼링
	// 1. 생성
	m_hBackBit = ::CreateCompatibleBitmap(m_hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_hBackDC = ::CreateCompatibleDC(m_hDC);

	// 2. 연결
	::SelectObject(m_hBackDC, m_hBackBit);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init();
	if (!GET_SINGLE(ResourceManager)->Init())
		return false;
	GET_SINGLE(SceneManager)->Init();

	return true;
}

void Core::MainUpdate()
{   
	GET_SINGLE(TimeManager)->Update();
	{
		static float accumulator = 0.f;
		const float fixedDT = 1.f / 60.f; // 60fps 기준 물리 프레임

		accumulator += fDT;
		while (accumulator >= fixedDT)
		{
			GET_SINGLE(SceneManager)->FixedUpdate(fixedDT);
			GET_SINGLE(CollisionManager)->Update();
			accumulator -= fixedDT;
		}
	}
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(ResourceManager)->FmodUpdate();
	GET_SINGLE(SceneManager)->Update();
	//::SetWindowText(GetHwnd(), L"Deadlock");
	//GET_SINGLE(CollisionManager)->Update();	
}

void Core::MainRender()
{ 
	//::Rectangle(m_hBackDC, -1, -1, WINDOW_WIDTH +1 , WINDOW_HEIGHT +1 );
	::PatBlt(m_hBackDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLACKNESS);
	GET_SINGLE(SceneManager)->Render(m_hBackDC);
	::BitBlt(m_hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_hBackDC, 0, 0, SRCCOPY);
}



void Core::GameLoop()
{
	MainUpdate();
	MainRender();
	GET_SINGLE(SceneManager)->GetCurScene()->FlushEvent();
}


void Core::CleanUp()
{
	::DeleteDC(m_hBackDC);
	::DeleteObject(m_hBackBit);
	::ReleaseDC(m_hWnd, m_hDC);
	GET_SINGLE(ResourceManager)->Release();
	GET_SINGLE(SceneManager)->Release();
}
