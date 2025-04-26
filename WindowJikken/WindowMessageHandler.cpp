#include <iostream>
#include <windows.h>
# include <thread>

#include "WindowMessageHandler.h"

WindowMessageHandler::WindowMessageHandler()
{
    hWnd = NULL;
}

WindowMessageHandler::~WindowMessageHandler()
{

}

HWND WindowMessageHandler::CreateSpecifiedTitleClassWindow(const wchar_t* windowTitle, const wchar_t* className)
{
    WNDPROC winproc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return (LRESULT)0;

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    };
    const auto hInstance2 = GetModuleHandleW(nullptr);
       
    th_b = std::thread([=]
    {
        WNDCLASS wc = { };
        wc.lpfnWndProc = winproc;
        wc.hInstance = hInstance2;
        wc.lpszClassName = className;

        RegisterClass(&wc);

        hWnd = CreateWindowEx(
            0,                              // Optional window styles.
            className,
            windowTitle,
            WS_OVERLAPPEDWINDOW,            // Window style        
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,// Size and position
            NULL,       // Parent window    
            NULL,       // Menu
            hInstance2,  // Instance handle
            NULL        // Additional application data
        );

        ShowWindow(hWnd, SW_HIDE);

        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    });

    return 0;
}

void WindowMessageHandler::TerminateSpecifiedTitleClassWindow()
{
	auto ret = SendMessage(hWnd, WM_CLOSE, 0, 0);
	th_b.join();
}


