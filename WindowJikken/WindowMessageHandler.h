#pragma once
#include <windows.h>
#include <thread>
# include <functional>
# include <map>

class WindowMessageHandler
{
public:
	WindowMessageHandler();
	~WindowMessageHandler();

	HWND CreateSpecifiedTitleClassWindow(const wchar_t* windowTitle, const wchar_t* className);
	void RegisterFunction(UINT msg, std::function<void(HWND, UINT, WPARAM, LPARAM)> func);
	void TerminateSpecifiedTitleClassWindow();

private:
	HWND hWnd;

	std::thread MsgLoopThread;
	std::map<UINT, std::function<void(HWND, UINT, WPARAM, LPARAM)>> MessageFunctionMap;

	static LRESULT CALLBACK WndProcForGetThisPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};

