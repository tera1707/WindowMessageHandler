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

LogOnDesktop lod;
WindowMessageHandler wmh;

auto dispStateTable = std::map<WPARAM, std::wstring>
{
	{ PowerMonitorOff, L"PowerMonitorOff"},
	{ PowerMonitorOn , L"PowerMonitorOn" },
	{ PowerMonitorDim , L"PowerMonitorDim" },
};

auto powerSourceTable = std::map<WPARAM, std::wstring>
{
	{ PoAc  , L"PoAc"},
	{ PoDc  , L"PoDc"},
	{ PoHot  , L"PoHot" },
};
auto userPresenceTable = std::map<WPARAM, std::wstring>
{
	{ PowerUserPresent   , L"PowerUserPresent"},
	{ PowerUserNotPresent   , L"PowerUserNotPresent"},
	{ PowerUserInactive   , L"PowerUserInactive" },
};
auto powerSavingTable = std::map<WPARAM, std::wstring>
{
	{ 0    , L"Battery saver is off."},
	{ 1    , L"Battery saver is on. Save energy where possible."},
};
auto energySaverTable = std::map<WPARAM, std::wstring>
{
	{ ENERGY_SAVER_OFF    , L"ENERGY_SAVER_OFF"},
	{ ENERGY_SAVER_STANDARD    , L"ENERGY_SAVER_STANDARD"},
	{ ENERGY_SAVER_HIGH_SAVINGS    , L"ENERGY_SAVER_HIGH_SAVINGS" },
};

auto msgTable = std::map<WPARAM, std::wstring>{
	{ WTS_CONSOLE_CONNECT, L"WTS_CONSOLE_CONNECT"},
	{ WTS_CONSOLE_DISCONNECT, L"WTS_CONSOLE_DISCONNECT"},
	{ WTS_REMOTE_CONNECT, L"WTS_REMOTE_CONNECT" },
	{ WTS_REMOTE_DISCONNECT, L"WTS_REMOTE_DISCONNECT" },
	{ WTS_SESSION_LOGON, L"WTS_SESSION_LOGON" },
	{ WTS_SESSION_LOGOFF, L"WTS_SESSION_LOGOFF" },
	{ WTS_SESSION_LOCK, L"WTS_SESSION_LOCK" },//https://blog.kaorun55.com/entry/20080212/1202829851
	{ WTS_SESSION_UNLOCK, L"WTS_SESSION_UNLOCK" },
	{ WTS_SESSION_REMOTE_CONTROL, L"WTS_SESSION_REMOTE_CONTROL" },
	{ WTS_SESSION_CREATE, L"WTS_SESSION_CREATE" },
	{ WTS_SESSION_TERMINATE, L"WTS_SESSION_TERMINATE" },
};

int main()
{
	std::wcout << L"ウインドウメッセージ実験アプリを開始します。" << std::endl;
	std::wcout << L"※このConsoleにはログは出ません。デスクトップのmylog.logを見てください。" << std::endl;
	lod.WriteLine(L"ウインドウメッセージ実験アプリを開始します。");

	// メッセージ受信用見えないウインドウ作成
	wmh.CreateSpecifiedTitleClassWindow(L"MyWindowTitle1", L"MyClassName1");

	// メッセージ受信時イベント登録
	wmh.RegisterFunction(WM_APP + 1, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			lod.WriteLine(L"WM_APP + 1 -> closeします");
			wmh.RequestCloseSpecifiedTitleClassWindow();
		});

	wmh.RegisterFunction(WM_POWERBROADCAST, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/power/wm-powerbroadcast-messages
			// https://learn.microsoft.com/ja-jp/windows/win32/power/wm-powerbroadcast
			if (wp == PBT_POWERSETTINGCHANGE)
			{
				auto pbs = (POWERBROADCAST_SETTING*)lp;

				if (pbs->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_CONSOLE_DISPLAY_STATE {0}", dispStateTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_SESSION_DISPLAY_STATUS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_SESSION_DISPLAY_STATUS {0}", dispStateTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_BATTERY_PERCENTAGE_REMAINING)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_BATTERY_PERCENTAGE_REMAINING {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_ACDC_POWER_SOURCE)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_ACDC_POWER_SOURCE {0}", powerSourceTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_GLOBAL_USER_PRESENCE)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_GLOBAL_USER_PRESENCE {0}", userPresenceTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_LIDSWITCH_STATE_CHANGE {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_MONITOR_POWER_ON)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_MONITOR_POWER_ON {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_POWER_SAVING_STATUS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_POWER_SAVING_STATUS  {0}", powerSavingTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_ENERGY_SAVER_STATUS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_ENERGY_SAVER_STATUS {0}", energySaverTable[pbs->Data[0]]));
				}
				else if (pbs->PowerSetting == GUID_POWERSCHEME_PERSONALITY)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_POWERSCHEME_PERSONALITY {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_MIN_POWER_SAVINGS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_MIN_POWER_SAVINGS {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_MAX_POWER_SAVINGS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_MAX_POWER_SAVINGS {0}", pbs->Data[0]));
				}
				else if (pbs->PowerSetting == GUID_TYPICAL_POWER_SAVINGS)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_TYPICAL_POWER_SAVINGS {0}", pbs->Data[0]));
				}
			}
			else if (wp == PBT_APMSUSPEND)
			{
				// サスペンド(スリープ)
				lod.WriteLine(L"PBT_APMSUSPEND");
			}
			else if (wp == PBT_APMRESUMESUSPEND)
			{
				// レジューム
				lod.WriteLine(L"PBT_APMRESUMESUSPEND");
			}
			else if (wp == PBT_APMRESUMEAUTOMATIC)
			{
				// レジューム(Automatic？)
				lod.WriteLine(L"PBT_APMRESUMEAUTOMATIC");
			}
			else if (wp == PBT_APMBATTERYLOW)
			{
				// レジューム(Automatic？)
				lod.WriteLine(L"PBT_APMBATTERYLOW");
			}
			else if (wp == PBT_APMPOWERSTATUSCHANGE)
			{
				// レジューム(Automatic？)
				lod.WriteLine(L"PBT_APMPOWERSTATUSCHANGE");
			}
		});

	wmh.RegisterFunction(WM_QUERYENDSESSION, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-queryendsession
			lod.WriteLine(L"WM_QUERYENDSESSION");
		});

	wmh.RegisterFunction(WM_ENDSESSION, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-endsession
			lod.WriteLine(L"WM_ENDSESSION ");
		}); 

	wmh.RegisterFunction(WM_USERCHANGED, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-userchanged
			lod.WriteLine(L"WM_USERCHANGED ");
		});

	wmh.RegisterFunction(WM_WTSSESSION_CHANGE, [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			//lpはセッションIDが入っている
			// https://learn.microsoft.com/ja-jp/windows/win32/termserv/wm-wtssession-change
			//https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wtssession_notification
			if (msgTable.count(wp) > 0)
				lod.WriteLine((L"WM_WTSSESSION_CHANGE " + msgTable[wp] + L" セッションNo." + std::to_wstring(lp)).c_str());
		});

	// ウインドウ終了待ち
	wmh.WaitForCloseSpecifiedTitleClassWindow();

	std::wcout << L"ウインドウを閉じて正常終了しました。" << std::endl;
	lod.WriteLine(L"ウインドウを閉じて正常終了しました。");

	return 0;
}

