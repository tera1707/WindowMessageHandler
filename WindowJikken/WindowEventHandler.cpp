#include <windows.h>
#include <string>
#include <Dbt.h>
#include "WindowEventHandler.h"
#include "LogOnDesktop.h"

WindowEventHandler::WindowEventHandler()
{

}

WindowEventHandler::~WindowEventHandler()
{

}

void AnalyzeDeviceType(LPARAM lp)
{
	DEV_BROADCAST_HDR* pHdr = reinterpret_cast<DEV_BROADCAST_HDR*>(lp);
	if (pHdr)
	{
		switch (pHdr->dbch_devicetype)
		{
		case DBT_DEVTYP_DEVICEINTERFACE:
		{
			DEV_BROADCAST_DEVICEINTERFACE* pDevInterface = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(pHdr);
			lod.WriteLine(std::format(L"DBT_DEVICEARRIVAL: Device Interface Name: {}", pDevInterface->dbcc_name));
			break;
		}
		case DBT_DEVTYP_VOLUME:
		{
			DEV_BROADCAST_VOLUME* pVolume = reinterpret_cast<DEV_BROADCAST_VOLUME*>(pHdr);
			lod.WriteLine(std::format(L"DBT_DEVICEARRIVAL: Volume Mask: {}", pVolume->dbcv_unitmask));
			break;
		}
		case DBT_DEVTYP_PORT:
		{
			DEV_BROADCAST_PORT* pPort = reinterpret_cast<DEV_BROADCAST_PORT*>(pHdr);
			lod.WriteLine(std::format(L"DBT_DEVICEARRIVAL: Port Name: {}", pPort->dbcp_name));
			break;
		}
		case DBT_DEVTYP_OEM:
		{
			DEV_BROADCAST_OEM* pOem = reinterpret_cast<DEV_BROADCAST_OEM*>(pHdr);
			lod.WriteLine(std::format(L"DBT_DEVICEARRIVAL: OEM Identifier: {}, OEM Type: {}", pOem->dbco_identifier, pOem->dbco_suppfunc));
			break;
		}
		default:
			lod.WriteLine(L"DBT_DEVICEARRIVAL: Unknown device type.");
			break;
		}
	}
	else
	{
		lod.WriteLine(L"DBT_DEVICEARRIVAL: No device information available.");
	}
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
				auto pbs = ((PPOWERBROADCAST_SETTING)lp);

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
				else if (pbs->PowerSetting == GUID_SESSION_USER_PRESENCE)
				{
					lod.WriteLine(std::format(L"PBT_POWERSETTINGCHANGE GUID_SESSION_USER_PRESENCE  {0}", userPresenceTable[pbs->Data[0]]));
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

	wmh.RegisterFunction(WM_DEVICECHANGE, [&](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/devio/wm-devicechange
			// https://learn.microsoft.com/ja-jp/windows-hardware/drivers/kernel/processing-wm_devicechange-messages
			lod.WriteLine(std::format(L"WM_DEVICECHANGE {}", deviceChangeTable[wp]));

			if (wp == DBT_DEVNODES_CHANGED)
			{
				// 詳しい情報をとるには、RegisterDeviceNotification関数を使用してデバイス通知を登録する必要があります
				// https://learn.microsoft.com/en-us/windows/win32/devio/dbt-devnodes-changed
				// でもDBT_DEVNODES_CHANGEDは、lpが固定で0だから情報は載ってこないっぽい
			}
			if (wp == DBT_DEVICEARRIVAL)
			{
				AnalyzeDeviceType(lp);
			}
			if (wp == DBT_DEVICEREMOVECOMPLETE)
			{
				AnalyzeDeviceType(lp);
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
