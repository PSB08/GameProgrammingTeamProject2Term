#pragma once
class TimeManager
{
	DECLARE_SINGLE(TimeManager);
public:
	void Init();
	void Update();
public:
	float GetDT() const { return m_deltaTime; }
private:
	// Delta Time
	LARGE_INTEGER m_llPrevCnt = {};
	LARGE_INTEGER m_llCurCnt = {};
	LARGE_INTEGER m_llFrequency = {};
	
	float m_deltaTime = 0.f;
	
	// FPS
	UINT m_fps = 0;
	UINT m_frameCnt = 0;
	float m_frameTime = 0.f;
};

