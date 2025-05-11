#pragma once
#include <functional>
#include <dbt.h>
#include "WindowMessageHandler.h"
#include "LogOnDesktop.h"

#pragma comment(lib, "WtsApi32.lib")

#ifdef _DEBUG
extern LogOnDesktop lod;
#endif

enum LockStatus
{
	Locked,
	Unlocked,
};

class WindowEventHandler
{
public:
	WindowEventHandler();
	~WindowEventHandler();

	void WaitForEvent();

	void RegisterOnBatteryRemainChanged(std::function<void(int battRemain)>);

	void RegisterOnLockChanged(std::function<void(LockStatus lockState)>);


private:
#if _DEBUG
	//LogOnDesktop lodEvth = LogOnDesktop(L"WinEventLog_EventHandler.log");

	std::map<WPARAM, std::wstring> dispStateTable = std::map<WPARAM, std::wstring>
	{
		{ PowerMonitorOff, L"PowerMonitorOff"},
		{ PowerMonitorOn , L"PowerMonitorOn" },
		{ PowerMonitorDim , L"PowerMonitorDim" },
	};

	std::map<WPARAM, std::wstring> powerSourceTable = std::map<WPARAM, std::wstring>
	{
		{ PoAc  , L"PoAc"},
		{ PoDc  , L"PoDc"},
		{ PoHot  , L"PoHot" },
	};
	std::map<WPARAM, std::wstring> userPresenceTable = std::map<WPARAM, std::wstring>
	{
		{ PowerUserPresent   , L"PowerUserPresent"},
		{ PowerUserNotPresent   , L"PowerUserNotPresent"},
		{ PowerUserInactive   , L"PowerUserInactive" },
	};
	std::map<WPARAM, std::wstring> powerSavingTable = std::map<WPARAM, std::wstring>
	{
		{ 0    , L"Battery saver is off."},
		{ 1    , L"Battery saver is on. Save energy where possible."},
	};
	std::map<WPARAM, std::wstring> energySaverTable = std::map<WPARAM, std::wstring>
	{
		{ ENERGY_SAVER_OFF    , L"ENERGY_SAVER_OFF"},
		{ ENERGY_SAVER_STANDARD    , L"ENERGY_SAVER_STANDARD"},
		{ ENERGY_SAVER_HIGH_SAVINGS    , L"ENERGY_SAVER_HIGH_SAVINGS" },
	};

	std::map<WPARAM, std::wstring> msgTable = std::map<WPARAM, std::wstring>
	{
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

	std::map<WPARAM, std::wstring> deviceChangeTable = std::map<WPARAM, std::wstring>
	{
		{ DBT_DEVNODES_CHANGED, L"DBT_DEVNODES_CHANGED" },
		{ DBT_QUERYCHANGECONFIG, L"DBT_QUERYCHANGECONFIG" },
		{ DBT_CONFIGCHANGED, L"DBT_CONFIGCHANGED" },
		{ DBT_CONFIGCHANGECANCELED, L"DBT_CONFIGCHANGECANCELED" },
		{ DBT_DEVICEARRIVAL, L"DBT_DEVICEARRIVAL" },
		{ DBT_DEVICEQUERYREMOVE, L"DBT_DEVICEQUERYREMOVE" },
		{ DBT_DEVICEQUERYREMOVEFAILED, L"DBT_DEVICEQUERYREMOVEFAILED" },
		{ DBT_DEVICEREMOVEPENDING, L"DBT_DEVICEREMOVEPENDING" },
		{ DBT_DEVICEREMOVECOMPLETE, L"DBT_DEVICEREMOVECOMPLETE" },
		{ DBT_DEVICETYPESPECIFIC, L"DBT_DEVICETYPESPECIFIC" },
		{ DBT_CUSTOMEVENT, L"DBT_CUSTOMEVENT" }
	};
#endif

    #ifdef _DEBUG
    #endif
	WindowMessageHandler wmh;

	std::function<void(int battRemain)> OnBatteryRemainChanged;
	std::function<void(LockStatus lockState)> OnLockStateChanged;


};

