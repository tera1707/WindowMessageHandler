#pragma once
#include <windows.h>
#include <thread>

class WindowMessageHandler
{
public:
	WindowMessageHandler();
	~WindowMessageHandler();

	HWND CreateSpecifiedTitleClassWindow(const wchar_t* windowTitle, const wchar_t* className);
	//int ShowSpecifiedTitleClassWindow(HWND hWnd);

	void TerminateSpecifiedTitleClassWindow();



private:
	//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::thread th_b;
	HWND hWnd;
};

