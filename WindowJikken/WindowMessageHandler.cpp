
#include "WindowMessageHandler.h"
#include <iostream>

WindowMessageHandler::WindowMessageHandler()
{
    hWnd = NULL;
}

WindowMessageHandler::~WindowMessageHandler()
{

}

/// <summary>
/// WM_���b�Z�[�W�ɑΉ����鏈���̓o�^
/// </summary>
/// <param name="msg"></param>
/// <param name="func"></param>
void WindowMessageHandler::RegisterFunction(UINT msg, std::function<void(HWND, UINT, WPARAM, LPARAM)> func)
{
    MessageFunctionMap[msg] = func;
}

/// <summary>
/// �N���X���(this�|�C���^)���擾���邽�߂�WndProc�ɂ��Ԃ����֐�
/// WM_CREATE��lParam�Ƃ��Ă킽���Ă���this�|�C���^��GWLP_USERDATA�ŕۑ�����
/// �ۑ��ȍ~�́A����this�|�C���^���g���B
/// �g�����F���̃N���X�ł́A�w���Msg�ɑΉ������֐����ĂԂ��߂�map�擾�Ɏg���B
/// </summary>
LRESULT CALLBACK WindowMessageHandler::WndProcForGetThisPtr(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowMessageHandler* pThis = (WindowMessageHandler*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (uMsg == WM_CREATE)
    {
        pThis = (WindowMessageHandler*)(((CREATESTRUCT*)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
    }

    if (pThis)
    {
        if (pThis->MessageFunctionMap.count(uMsg) > 0)
            pThis->MessageFunctionMap[uMsg](hwnd, uMsg, wParam, lParam);

        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// �E�C���h�E�v���V�[�W���\
/// </summary>
LRESULT CALLBACK WindowMessageHandler::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return (LRESULT)0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// <summary>
/// �w��̃E�C���h�E�^�C�g���A�E�C���h�E�N���X���ŃE�C���h�E���쐬���A�����Ȃ���Ԃŗ����グ��
/// </summary>
HWND WindowMessageHandler::CreateSpecifiedTitleClassWindow(const wchar_t* windowTitle, const wchar_t* className)
{
    const auto hInstance2 = GetModuleHandleW(nullptr);
       
    MsgLoopThread = std::thread([=]
    {
        WNDCLASS wc = { };
        wc.lpfnWndProc = WndProcForGetThisPtr;
        wc.hInstance = hInstance2;
        wc.lpszClassName = className;

        RegisterClass(&wc);

        hWnd = CreateWindowEx(
            0,                              // Optional window styles.
            className,
            windowTitle,
            WS_OVERLAPPEDWINDOW,            // Window style        
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,// Size and position
            NULL,       // Parent window    
            NULL,       // Menu
            hInstance2,  // Instance handle
            (WindowMessageHandler*)this        // Additional application data
        );

        ShowWindow(hWnd, SW_HIDE);

        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    });

    return 0;
}

/// <summary>
/// �I������
/// </summary>
void WindowMessageHandler::TerminateSpecifiedTitleClassWindow()
{
    // �E�C���h�E�����Ă��Ȃ��ꍇ�͕��ɍs��
	auto ret = SendMessage(hWnd, WM_CLOSE, 0, 0);

    // �E�C���h�E��Msg���[�v�p�X���b�h�̏I���҂�
	MsgLoopThread.join();
}


