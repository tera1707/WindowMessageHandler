using System.Runtime.InteropServices;

using static WindowJikkenCs.Model.NativeMethods;

namespace WindowJikkenCs.Model;

internal class WindowMessageHandlerCs
{
    private string szAppName = "MyWindowClassName";
    private string szWindowTitle = "MyWindowTitle";

    private IntPtr registerConsoleDisplayHandle = IntPtr.Zero;
    private IntPtr registerPowerSavingHandle = IntPtr.Zero;
    private IntPtr registerEnergySaverHandle = IntPtr.Zero;

    // ウインドウメッセージ毎に、呼び出すメソッドを登録しておくDictionary
    private Dictionary<uint, Action<IntPtr, uint, IntPtr, IntPtr>> _actinDic = new();

    public void CreateSpecifiedTitleClassWindow()
    {
        MSG msg;
        int bRet;

        var hCurInst = (UInt16)GetModuleHandle(string.Empty);

        if (RegisterWindowClass(hCurInst) == 0)
            return;

        // SW_HIDEにして、見えないウインドウにする
        if (InitInstance(hCurInst, SW_HIDE) == 0)
            return;

        while ((bRet = GetMessage(out msg, NULL, 0, 0)) != 0)
        {
            if (bRet == -1)
            {
                break;
            }
            else
            {
                TranslateMessage(ref msg);
                DispatchMessage(ref msg);
            }
        }
    }

    // ウィンドウクラス登録
    UInt16 RegisterWindowClass(UInt16 hInst)
    {
        var wcx = new WNDCLASSEX()
        {
            cbSize = Marshal.SizeOf(typeof(WNDCLASSEX)),
            style = CS_VREDRAW | CS_HREDRAW,
            lpfnWndProc = WndProc,
            cbClsExtra = 0,
            cbWndExtra = 0,
            hInstance = hInst,
            hIcon = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
            hCursor = LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
            hbrBackground = GetStockObject(WHITE_BRUSH),
            lpszMenuName = null,
            lpszClassName = szAppName,
            hIconSm = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
        };

        return RegisterClassEx(ref wcx);
    }

    // インスタンス作成
    System.Int32 InitInstance(IntPtr hInst, int nCmdShow)
    {
        var hWnd = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,
            szAppName,
            szWindowTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            hInst,
            NULL
        );

        if (hWnd == NULL) return FALSE;

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    public void RegisterFunction(uint msg, Action<IntPtr, uint, IntPtr, IntPtr> actinDic)
    {
        _actinDic.Add(msg, actinDic);
    }

    // コールバック
    private IntPtr WndProc(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam)
    {
        if (_actinDic.TryGetValue(msg, out var action))
            action.Invoke(hWnd, msg, wParam, lParam);

        switch (msg)
        {
            case WM_CREATE:
                WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_ALL_SESSIONS);
                registerConsoleDisplayHandle = RegisterPowerSettingNotification(hWnd, ref GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
                registerPowerSavingHandle = RegisterPowerSettingNotification(hWnd, ref GUID_POWER_SAVING_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);
                registerEnergySaverHandle = RegisterPowerSettingNotification(hWnd, ref GUID_ENERGY_SAVER_STATUS, DEVICE_NOTIFY_WINDOW_HANDLE);
                break;
            case WM_DESTROY:
                UnregisterPowerSettingNotification(registerConsoleDisplayHandle);
                UnregisterPowerSettingNotification(registerPowerSavingHandle);
                UnregisterPowerSettingNotification(registerEnergySaverHandle);

                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return NULL;
    }
}
