#pragma once
//include "Object.h"
class Core
{
	DECLARE_SINGLE(Core);
public:
	bool Init(HWND _hWnd);
	void GameLoop();
	void CleanUp();
public:
	const HWND& GetHwnd() const { return m_hWnd; }
	const HDC& GetMainDC() const { return m_hDC; }
private:
	void MainUpdate();
	void MainRender();
private:
	HWND m_hWnd;
	HDC  m_hDC;
	//Object m_obj;
	// 더블버퍼링
	HBITMAP m_hBackBit;
	HDC  m_hBackDC;
};



//class Core
//{
//private:
//	Core() {}
//public:
//	static Core* GetInst()
//	{
//		if (nullptr == m_pInst)
//			m_pInst = new Core;
//		return m_pInst;
//	}
//	static void DestroyInst()
//	{
//
//	}
//private:
//	static Core m_pInst;
//};


//private:
//	Core() {}
//public:
//	static Core* GetInst()
//	{
//		static Core inst;
//		return &inst;
//	}