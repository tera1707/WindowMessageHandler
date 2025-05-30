
#include "WindowMessageHandler.h"
#include <iostream>
#include <wtsapi32.h>

//WTSRegisterSessionNotification を使うために必要
#pragma comment(lib, "Wtsapi32.lib")

WindowMessageHandler::WindowMessageHandler()
{
    hWnd = NULL;
    hPowerNotify = NULL;
}

WindowMessageHandler::~WindowMessageHandler()
{
}

/// <summary>
/// WM_メッセージに対応する処理の登録
/// </summary>
/// <param name="msg"></param>
/// <param name="func"></param>
void WindowMessageHandler::RegisterFunction(UINT msg, std::function<void(HWND, UINT, WPARAM, LPARAM)> func)
{
    MessageFunctionMap[msg] = func;
}

/// <summary>
/// クラス情報(thisポインタ)を取得するためにWndProcにかぶせた関数
/// WM_CREATEでlParamとしてわたってきたthisポインタをGWLP_USERDATAで保存する
/// 保存以降は、そのthisポインタを使う。
/// 使い道：このクラスでは、指定のMsgに対応した関数を呼ぶためのmap取得に使う。
/// </summary>
LRESULT CALLBACK WindowMessageHandler::WndProcForGetThisPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowMessageHandler* pThis = (WindowMessageHandler*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (uMsg == WM_CREATE)
    {
        pThis = (WindowMessageHandler*)(((CREATESTRUCT*)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
    }

    if (pThis)
    {
        if (pThis->MessageFunctionMap.count(uMsg) > 0)
            pThis->MessageFunctionMap[uMsg](hwnd, uMsg, wParam, lParam);

        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// ウインドウプロシージャ―
/// </summary>
LRESULT CALLBACK WindowMessageHandler::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        hPowerNotify = RegisterPowerSettingNotification(hwnd, &GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
        //WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_THIS_SESSION); // 自分のセッション(ユーザー)のイベントのみ取れればよいならこちら
        WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_ALL_SESSIONS);   // 自分だけでなく全セッションのイベントを取る場合はこちら
        RegisterPowerSettingNotification(hwnd, &GUID_POWER_SAVING_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);
        RegisterPowerSettingNotification(hwnd, &GUID_ENERGY_SAVER_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);
        break;
    }
    case WM_DESTROY:
    {
        if (hPowerNotify != NULL)
            UnregisterPowerSettingNotification(hPowerNotify);

        WTSUnRegisterSessionNotification(hwnd);

        PostQuitMessage(0);
        return (LRESULT)0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// 指定のウインドウタイトル、ウインドウクラス名でウインドウを作成し、見えない状態で立ち上げる
/// </summary>
HWND WindowMessageHandler::CreateSpecifiedTitleClassWindow(const wchar_t* windowTitle, const wchar_t* className)
{
    const auto hInstance2 = GetModuleHandleW(nullptr);
       
    MsgLoopThread = std::thread([=]
    {
        WNDCLASS wc = { };
        wc.lpfnWndProc = WndProcForGetThisPtr;
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
            (WindowMessageHandler*)this        // Additional application data
        );

        ShowWindow(hWnd, SW_HIDE);

        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        OutputDebugString(L"Thread Finished.");
    });

    return 0;
}

/// <summary>
/// 終了処理
/// </summary>
void WindowMessageHandler::RequestCloseSpecifiedTitleClassWindow()
{
    // ウインドウが閉じていない場合は閉じに行く
	auto ret = SendMessage(hWnd, WM_CLOSE, 0, 0);
}

void WindowMessageHandler::WaitForCloseSpecifiedTitleClassWindow()
{
    // ウインドウのMsgループ用スレッドの終了待ち
    MsgLoopThread.join();
}

