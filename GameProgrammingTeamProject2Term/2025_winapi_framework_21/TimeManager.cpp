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
	}
}
