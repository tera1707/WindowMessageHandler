#include <windows.h>
#include <string>
#include "WindowEventHandler.h"
#include "LogOnDesktop.h"

WindowEventHandler::WindowEventHandler()
{

}

WindowEventHandler::~WindowEventHandler()
{

}

void WindowEventHandler::WaitForEvent()
{
	// メッセージ受信用見えないウインドウ作成
	wmh.CreateSpecifiedTitleClassWindow(L"MyWindowTitle1", L"MyClassName1");

	// メッセージ受信時イベント登録
	wmh.RegisterFunction(WM_APP + 1, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			lod.WriteLine(L"WM_APP + 1 -> closeします");
			wmh.RequestCloseSpecifiedTitleClassWindow();
		});

	wmh.RegisterFunction(WM_POWERBROADCAST, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
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
					auto batteryRemain = (int)pbs->Data[0];
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_BATTERY_PERCENTAGE_REMAINING {0}", batteryRemain));

					if ((bool)OnBatteryRemainChanged)
						OnBatteryRemainChanged(batteryRemain);
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

	wmh.RegisterFunction(WM_QUERYENDSESSION, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-queryendsession
			lod.WriteLine(L"WM_QUERYENDSESSION");
		});

	wmh.RegisterFunction(WM_ENDSESSION, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/shutdown/wm-endsession
			lod.WriteLine(L"WM_ENDSESSION ");
		});

	wmh.RegisterFunction(WM_USERCHANGED, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-userchanged
			lod.WriteLine(L"WM_USERCHANGED ");
		});

	wmh.RegisterFunction(WM_WTSSESSION_CHANGE, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			//lpはセッションIDが入っている
			// https://learn.microsoft.com/ja-jp/windows/win32/termserv/wm-wtssession-change
			//https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wtssession_notification
			// 自分のセッション以外のセッション系イベントを見る必要がない場合は、
			// WindowMessageHandler側でWTSRegisterSessionNotification()を呼ぶときの第二引数をNOTIFY_FOR_THIS_SESSIONに直す。

			if (msgTable.count(wp) > 0)
				lod.WriteLine((L"WM_WTSSESSION_CHANGE " + msgTable[wp] + L" セッションNo." + std::to_wstring(lp)).c_str());

			if (!(bool)OnLockStateChanged)
				return;

			if (wp == WTS_SESSION_LOCK)
			{
				OnLockStateChanged(LockStatus::Locked);
			}
			else if (wp == WTS_SESSION_UNLOCK)
			{
				OnLockStateChanged(LockStatus::Unlocked);
			}
		});

	// ウインドウ終了待ち
	wmh.WaitForCloseSpecifiedTitleClassWindow();
}

void WindowEventHandler::RegisterOnBatteryRemainChanged(std::function<void(int battRemain)> action)
{
	OnBatteryRemainChanged = action;
}

void WindowEventHandler::RegisterOnLockChanged(std::function<void(LockStatus lockState)> action)
{
	OnLockStateChanged = action;
}
