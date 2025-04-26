#include <iostream>
#include <windows.h>
#include "WindowMessageHandler.h"
#include <thread>
#include <map>
#include <string>

// 初めての Windows プログラム
// https://learn.microsoft.com/ja-jp/windows/win32/learnwin32/your-first-windows-program
// http://kaitei.net/winapi/creating-windows/
// https://stackoverflow.com/questions/14292803/can-i-have-main-window-procedure-as-a-lambda-in-winmain
// https://akatukisiden.wordpress.com/2016/02/14/c-%E3%81%AB%E3%82%88%E3%82%8B-windows-%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%AD%A6%E7%BF%92-1-6-%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7/

void log(HWND hwnd, UINT msg)
{
	wchar_t buf[256];
	GetWindowText(hwnd, buf, sizeof(buf)/ sizeof(buf[0]));
	auto title = std::wstring(buf);
	OutputDebugString((title + L" : " + std::to_wstring(msg) + L"\r\n").c_str());
}

int main()
{
	WindowMessageHandler windowMessageHandler1;
    WindowMessageHandler windowMessageHandler2;

	windowMessageHandler1.RegisterFunction(WM_APP + 1, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { log(hwnd, msg); });
	windowMessageHandler1.RegisterFunction(WM_APP + 2, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { log(hwnd, msg); });
	windowMessageHandler2.RegisterFunction(WM_APP + 1, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { log(hwnd, msg); });
	windowMessageHandler2.RegisterFunction(WM_APP + 2, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { log(hwnd, msg); });
	windowMessageHandler2.RegisterFunction(WM_APP + 3, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { log(hwnd, msg); });

    windowMessageHandler1.CreateSpecifiedTitleClassWindow(L"MyWindowTitle1", L"MyClassName1");
    windowMessageHandler2.CreateSpecifiedTitleClassWindow(L"MyWindowTitle2", L"MyClassName2");
    
    // 終了させないよう入力待ちにする
    int a;
    std::cin >> a;

    // ウインドウ終了処理
	windowMessageHandler1.TerminateSpecifiedTitleClassWindow();
    windowMessageHandler2.TerminateSpecifiedTitleClassWindow();

	return 0;
}

