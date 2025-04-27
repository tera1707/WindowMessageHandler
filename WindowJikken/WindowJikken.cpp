#include <iostream>
#include <windows.h>
#include <thread>
#include <map>
#include <string>
#include "WindowMessageHandler.h"
#include "LogOnDesktop.h"

// 初めての Windows プログラム
// https://learn.microsoft.com/ja-jp/windows/win32/learnwin32/your-first-windows-program
// http://kaitei.net/winapi/creating-windows/
// https://stackoverflow.com/questions/14292803/can-i-have-main-window-procedure-as-a-lambda-in-winmain
// https://akatukisiden.wordpress.com/2016/02/14/c-%E3%81%AB%E3%82%88%E3%82%8B-windows-%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%AD%A6%E7%BF%92-1-6-%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7/

//void log(HWND hwnd, UINT msg)
//{
//	wchar_t buf[256];
//	GetWindowText(hwnd, buf, sizeof(buf)/ sizeof(buf[0]));
//	auto title = std::wstring(buf);
//	OutputDebugString((title + L" : " + std::to_wstring(msg) + L"\r\n").c_str());
//}

LogOnDesktop lod;
WindowMessageHandler wmh;

int main()
{
	std::wcout << L"ウインドウメッセージ実験アプリを開始します。" << std::endl;
	lod.WriteLine(L"ウインドウメッセージ実験アプリを開始します。");

	// メッセージ受信用見えないウインドウ作成
	wmh.CreateSpecifiedTitleClassWindow(L"MyWindowTitle1", L"MyClassName1");

	// メッセージ受信時イベント登録
	wmh.RegisterFunction(WM_APP + 1, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			//log(hwnd, msg);
			lod.WriteLine(L"WM_APP + 1");
			wmh.RequestCloseSpecifiedTitleClassWindow();
		});

	wmh.RegisterFunction(WM_POWERBROADCAST, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/power/wm-powerbroadcast-messages
			// https://learn.microsoft.com/ja-jp/windows/win32/power/wm-powerbroadcast
			//log(hwnd, msg);

			if (wp == PBT_POWERSETTINGCHANGE)
			{
				auto pbs = (POWERBROADCAST_SETTING*)lp;

				if (pbs->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				{
					// ディスプレイON/OFF
					if (pbs->Data != 0)
						lod.WriteLine(L"ディスプレイON");
					else 
						lod.WriteLine(L"ディスプレイOFF");
				}
			}
			else if (wp == PBT_APMSUSPEND)
			{
				// サスペンド(スリープ)
				lod.WriteLine(L"サスペンド(スリープ)");
			}
			else if (wp == PBT_APMRESUMESUSPEND)
			{
				// レジューム
				lod.WriteLine(L"サスペンド");
			}
			else if (wp == PBT_APMRESUMEAUTOMATIC)
			{
				// レジューム(Automatic？)
				lod.WriteLine(L"PBT_APMRESUMEAUTOMATIC");
			}

		});

	wmh.RegisterFunction(WM_QUERYENDSESSION, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-queryendsession
			//log(hwnd, msg);
			lod.WriteLine(L"WM_QUERYENDSESSION");
		});

	wmh.RegisterFunction(WM_ENDSESSION, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-endsession
			//log(hwnd, msg);
			lod.WriteLine(L"WM_ENDSESSION ");
		}); 

	wmh.RegisterFunction(WM_USERCHANGED, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-userchanged
			//log(hwnd, msg);
			lod.WriteLine(L"WM_USERCHANGED ");
		});

	wmh.RegisterFunction(WM_WTSSESSION_CHANGE, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			//lpはセッションIDが入っている
			// https://learn.microsoft.com/ja-jp/windows/win32/termserv/wm-wtssession-change
			//log(hwnd, msg);
			lod.WriteLine(L"WM_WTSSESSION_CHANGE ");

			auto msgTable = std::map<WPARAM, std::wstring>{
				{ WTS_CONSOLE_CONNECT, L"WTS_CONSOLE_CONNECT"},
				{ WTS_CONSOLE_DISCONNECT, L"WTS_CONSOLE_DISCONNECT"},
				{ WTS_REMOTE_CONNECT, L"WTS_REMOTE_CONNECT" },
				{ WTS_REMOTE_DISCONNECT, L"WTS_REMOTE_DISCONNECT" },
				{ WTS_SESSION_LOGON, L"WTS_SESSION_LOGON" },
				{ WTS_SESSION_LOGON, L"WTS_SESSION_LOGOFF" },
				{ WTS_SESSION_LOCK, L"WTS_SESSION_LOCK" },//https://blog.kaorun55.com/entry/20080212/1202829851
				{ WTS_SESSION_UNLOCK, L"WTS_SESSION_UNLOCK" },
				{ WTS_SESSION_REMOTE_CONTROL, L"WTS_SESSION_REMOTE_CONTROL" },
				{ WTS_SESSION_CREATE, L"WTS_SESSION_CREATE" },
				{ WTS_SESSION_TERMINATE, L"WTS_SESSION_TERMINATE" },
			};

			//https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wtssession_notification
			//auto sessionIdInfo = (PWTSSESSION_NOTIFICATION)lp;

			if (msgTable.count(wp) > 0)
				lod.WriteLine((L" " + msgTable[wp] + L" セッション：" + std::to_wstring(lp)).c_str());
		});

	// ウインドウ終了待ち
	wmh.WaitForCloseSpecifiedTitleClassWindow();

	std::wcout << L"ウインドウを閉じて正常終了しました。" << std::endl;
	lod.WriteLine(L"ウインドウを閉じて正常終了しました。");

	return 0;
}

