#pragma once
# include <functional>
#include "WindowMessageHandler.h"
#include "LogOnDesktop.h"

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
#endif

	WindowMessageHandler wmh;

	std::function<void(int battRemain)> OnBatteryRemainChanged;
	std::function<void(LockStatus lockState)> OnLockStateChanged;


};

