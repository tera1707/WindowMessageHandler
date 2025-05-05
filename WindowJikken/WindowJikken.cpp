#include <iostream>
#include <windows.h>
#include <string>
#include "LogOnDesktop.h"
#include "WindowEventHandler.h"

// 初めての Windows プログラム
// https://learn.microsoft.com/ja-jp/windows/win32/learnwin32/your-first-windows-program
// http://kaitei.net/winapi/creating-windows/
// https://stackoverflow.com/questions/14292803/can-i-have-main-window-procedure-as-a-lambda-in-winmain
// https://akatukisiden.wordpress.com/2016/02/14/c-%E3%81%AB%E3%82%88%E3%82%8B-windows-%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%AD%A6%E7%BF%92-1-6-%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7/

LogOnDesktop lod(L"mylog.log");

int main()
{
	WindowEventHandler weh;

	std::wcout << L"ウインドウメッセージ実験アプリを開始します。" << std::endl;
	lod.WriteLine(L"ウインドウメッセージ実験アプリを開始します。");
	std::wcout << L"※このConsoleにはログは出ません。デスクトップのmylog.logを見てください。" << std::endl;

	weh.RegisterOnBatteryRemainChanged([&](int battRemain)
		{
			lod.WriteLine(std::format(L"バッテリー残量：{0} %", battRemain));
		});

	weh.RegisterOnLockChanged([&](LockStatus lockState)
		{
			lod.WriteLine(std::format(L"ロック状態：{0}", lockState == LockStatus::Locked ? L"Locked" : L"Unlocked"));
		});

	weh.WaitForEvent();

	std::wcout << L"ウインドウを閉じて正常終了しました。" << std::endl;
	lod.WriteLine(L"ウインドウを閉じて正常終了しました。");

	return 0;
}

