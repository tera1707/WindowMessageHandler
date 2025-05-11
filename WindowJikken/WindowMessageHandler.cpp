
#include "WindowMessageHandler.h"
#include <iostream>
#include <wtsapi32.h>
#include <Dbt.h>
#include <initguid.h>
#include <Usbiodef.h>

//WTSRegisterSessionNotification ���g�����߂ɕK�v
#pragma comment(lib, "Wtsapi32.lib")

WindowMessageHandler::WindowMessageHandler()
{
    hWnd = NULL;
    hPowerNotify = NULL;
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
HDEVNOTIFY hDeviceNotify;
int ctr = 0;

void WindowMessageHandler::aaa(HWND hwnd, DWORD guid)
{
    // This GUID is for all USB serial host PnP drivers, but you can replace it 
    // with any valid device class guid.
    GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72,
                  0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = guid;
    NotificationFilter.dbcc_classguid = WceusbshGUID;//
    //NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;//
    //NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_HUB;//
    // ��������uDEVICE_NOTIFY_ALL_INTERFACE_CLASSES�v���ŌĂԂƁAUSB�L�[�{�[�h�̔���������DBT_DEVICEARRIVAL������悤�ɂȂ�BDEVICE_NOTIFY_ALL_INTERFACE_CLASSES�Ȃ��ŌĂ�ł�ARRIVAL��΂Ȃ��Ɨ��Ȃ��B
    // �������΂Ȃ��Ă��ADBT_DEVNODES_CHANGED�͗���
    //hDeviceNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
    hDeviceNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);

    if (NULL == hDeviceNotify)
    {
        ctr++;
        OutputDebugString(L"aaaa");
        return;
    }
}

/// <summary>
/// �E�C���h�E�v���V�[�W���\
/// </summary>
LRESULT CALLBACK WindowMessageHandler::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        hPowerNotify = RegisterPowerSettingNotification(hwnd, &GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
        //WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_THIS_SESSION); // �����̃Z�b�V����(���[�U�[)�̃C�x���g�̂ݎ���΂悢�Ȃ炱����
        WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_ALL_SESSIONS);   // ���������łȂ��S�Z�b�V�����̃C�x���g�����ꍇ�͂�����
        RegisterPowerSettingNotification(hwnd, &GUID_POWER_SAVING_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);
        RegisterPowerSettingNotification(hwnd, &GUID_ENERGY_SAVER_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);

        //aaa(hwnd, DBT_DEVTYP_DEVNODE);//����5�̓G���[�ɂȂ�
        //aaa(hwnd, DBT_DEVTYP_OEM);
        //aaa(hwnd, DBT_DEVTYP_VOLUME);
        //aaa(hwnd, DBT_DEVTYP_PORT);
        //aaa(hwnd, DBT_DEVTYP_NET);
        //aaa(hwnd, DBT_DEVTYP_HANDLE);
        aaa(hwnd, DBT_DEVTYP_DEVICEINTERFACE);
        printf("%d", ctr);
        break;
    }
    case WM_DESTROY:
    {
        if (hPowerNotify != NULL)
            UnregisterPowerSettingNotification(hPowerNotify);

        WTSUnRegisterSessionNotification(hwnd);

        PostQuitMessage(0);
        return (LRESULT)0;
    }
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
        OutputDebugString(L"Thread Finished.");
    });

    return 0;
}

/// <summary>
/// �I������
/// </summary>
void WindowMessageHandler::RequestCloseSpecifiedTitleClassWindow()
{
    // �E�C���h�E�����Ă��Ȃ��ꍇ�͕��ɍs��
	auto ret = SendMessage(hWnd, WM_CLOSE, 0, 0);
}

void WindowMessageHandler::WaitForCloseSpecifiedTitleClassWindow()
{
    // �E�C���h�E��Msg���[�v�p�X���b�h�̏I���҂�
    MsgLoopThread.join();
}

