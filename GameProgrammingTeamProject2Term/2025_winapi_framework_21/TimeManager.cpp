#include "pch.h"
#include "TimeManager.h"
#include "Core.h"
#include "InputManager.h"
void TimeManager::Init()
{
	////clock(); // -> FrameSync 1000ms == 1초
	//clock_t oldtime, curtime;
	//oldtime = clock();
	//while (true)
	//{
	//	curtime = clock();
	//	if (curtime - oldtime >= 1000)
	//	{
	//		cout << "1초";
	//	}
	//}
	//time(NULL);

	// 카운트 값 가져옴
	::QueryPerformanceCounter(&m_llPrevCnt);	

	// 1초에 1000만 반환
	::QueryPerformanceFrequency(&m_llFrequency);
}

void TimeManager::Update()
{
	::QueryPerformanceCounter(&m_llCurCnt);


	// dt 
	m_deltaTime = 
		(float)(m_llCurCnt.QuadPart - m_llPrevCnt.QuadPart)
		/ (float)(m_llFrequency.QuadPart);

	m_llPrevCnt = m_llCurCnt;

	m_frameCnt++;
	m_frameTime += m_deltaTime;
	if (m_frameTime >= 1.f)
	{
		m_fps = (UINT)(m_frameCnt / m_frameTime);
		m_frameTime = 0.f;
		m_frameCnt = 0;

		// 1초마다 한번 띄워봅시다.
		//std::wstring strFps = std::to_wstring(m_fps);
		//std::wstring strDt = std::to_wstring(m_deltaTime);
		//std::wstring str = L"FPS: " + strFps + L" DT: " + strDt;
		POINT mousePos = GET_MOUSEPOS;
		std::wstring str = 
			std::format(L"FPS: {0}, DT: {1:.6f}, Mouse: {2}, {3}"
			, m_fps, m_deltaTime, mousePos.x, mousePos.y);
		//::TextOut(GET_SINGLE(Core)->GetDC(), 0, 0, str.c_str(), str.length());
		::SetWindowText(GET_SINGLE(Core)->GetHwnd(), str.c_str());
	}
}
