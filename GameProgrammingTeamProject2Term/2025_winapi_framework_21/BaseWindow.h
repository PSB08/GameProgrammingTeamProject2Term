#pragma once
class BaseWindow
{
public:
	BaseWindow();
	~BaseWindow();
public:
	int Run(HINSTANCE hInstance, int nCmdShow);
private:
	ATOM MyRegisterClass();
	void createWindow();
	void showWindow(int nCmdShow);
	int MessageLoop();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HWND	  m_hWnd;
	HINSTANCE m_hInst;
};

