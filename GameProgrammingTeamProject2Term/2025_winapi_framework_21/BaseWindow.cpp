#include "pch.h"
#include "BaseWindow.h"
#include "Resource.h"
#include "Core.h"
BaseWindow::BaseWindow()
	: m_hWnd(nullptr)
	, m_hInst(nullptr)
{
}

BaseWindow::~BaseWindow()
{
}

int BaseWindow::Run(HINSTANCE hInstance, int nCmdShow)
{
    this->m_hInst = hInstance;
    this->MyRegisterClass();
    this->createWindow();
    this->showWindow(nCmdShow);

    if (!GET_SINGLE(Core)->Init(m_hWnd))
        MessageBox(m_hWnd, L"Core Init Error", L"ERROR", MB_OK);

    return this->MessageLoop();
}

ATOM BaseWindow::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = BaseWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"Deadlock";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    return ::RegisterClassExW(&wcex);
}

void BaseWindow::createWindow()
{
    int screenX = ::GetSystemMetrics(SM_CXSCREEN);
    int screenY = ::GetSystemMetrics(SM_CYSCREEN);
    int winposx = (screenX - WINDOW_WIDTH) / 2;
    int winposy = (screenY - WINDOW_HEIGHT) / 2;

    m_hWnd = ::CreateWindowW(L"Deadlock", L"Deadlock", 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        winposx, winposy, 
        WINDOW_WIDTH, WINDOW_HEIGHT, 
        nullptr, nullptr, 
        m_hInst, nullptr);

    RECT rtWindow = { winposx, winposy, winposx + WINDOW_WIDTH, winposy + WINDOW_HEIGHT };
    ::AdjustWindowRect(&rtWindow, WS_OVERLAPPEDWINDOW, false);

    ::MoveWindow(m_hWnd, winposx, winposy
        , rtWindow.right - rtWindow.left, rtWindow.bottom - rtWindow.top, true);
}

void BaseWindow::showWindow(int nCmdShow)
{
    ::ShowWindow(m_hWnd, nCmdShow);

}

int BaseWindow::MessageLoop()
{
    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        // 메시지 o
        if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        // 메시지 x
        else
        {
            GET_SINGLE(Core)->GameLoop();
        }
    }
    GET_SINGLE(Core)->CleanUp();
    return (int)msg.wParam;
}

LRESULT BaseWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xFFF0) == SC_MOVE)
            return 0;
        break;
    case WM_NCLBUTTONDOWN:
        if (wParam == HTCAPTION)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
     return DefWindowProc(hWnd, message, wParam, lParam);
}
