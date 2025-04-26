#include <iostream>
//#include <libloaderapi.h>
#include <windows.h>
#include "WindowMessageHandler.h"
#include <thread>

// 初めての Windows プログラム
// https://learn.microsoft.com/ja-jp/windows/win32/learnwin32/your-first-windows-program
// http://kaitei.net/winapi/creating-windows/

int main()
{
    std::cout << "Hello World!\n";


	WindowMessageHandler windowMessageHandler1;
    WindowMessageHandler windowMessageHandler2;

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

