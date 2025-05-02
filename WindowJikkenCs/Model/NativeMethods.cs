using System.Runtime.InteropServices;

namespace WindowJikkenCs.Model;

internal class NativeMethods
{
    internal const Int32 FALSE = 0;
    internal const Int32 TRUE = 1;

    // Window Class Styles
    internal const int CS_VREDRAW = 0x0001;
    internal const int CS_HREDRAW = 0x0002;

    // LoadImage
    internal const int LR_DEFAULTSIZE = 0x00000040;
    internal const int LR_SHARED = 0x00008000;
    internal const int IMAGE_ICON = 1;
    internal const int IMAGE_CURSOR = 2;
    internal const string IDI_APPLICATION = "#32512";
    internal const string IDC_ARROW = "#32512";

    // 拡張ウィンドウスタイル
    internal const uint WS_EX_WINDOWEDGE = 0x00000100;
    internal const uint WS_EX_CLIENTEDGE = 0x00000200;
    internal const uint WS_EX_OVERLAPPEDWINDOW = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;

    internal const uint WS_OVERLAPPED = 0x00000000;
    internal const uint WS_MAXIMIZEBOX = 0x00010000;
    internal const uint WS_MINIMIZEBOX = 0x00020000;
    internal const uint WS_THICKFRAME = 0x00040000;
    internal const uint WS_SYSMENU = 0x00080000;
    internal const uint WS_CAPTION = 0x00C00000;
    internal const uint WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

    // その他諸々
    internal const int WHITE_BRUSH = 0;
    internal const int SW_HIDE = 0;
    internal const int SW_SHOWNORMAL = 1;
    internal const int CW_USEDEFAULT = unchecked((int)0x80000000);
    internal const uint WM_DESTROY = 0x0002;

    internal const int WM_CREATE = 0x0001;
    internal const int WM_POWERBROADCAST = 0x0218;
    internal const int WM_QUERYENDSESSION = 0x0011;
    internal const int WM_ENDSESSION = 0x0016;
    internal const int WM_USERCHANGED = 0x0054;
    internal const int WM_WTSSESSION_CHANGE = 0x02B1;

    internal const int PBT_POWERSETTINGCHANGE = 0x8013;
    internal const int PBT_APMSUSPEND = 0x0004;
    internal const int PBT_APMRESUMESUSPEND = 0x0007;
    internal const int PBT_APMRESUMEAUTOMATIC = 0x0012;
    internal const int PBT_APMBATTERYLOW = 0x0009;
    internal const int PBT_APMPOWERSTATUSCHANGE = 0x000A;

    internal const int DEVICE_NOTIFY_WINDOW_HANDLE = 0x00000000;
    
    internal static Guid GUID_CONSOLE_DISPLAY_STATE = new Guid(0x6fe69556, 0x704a, 0x47a0, 0x8f, 0x24, 0xc2, 0x8d, 0x93, 0x6f, 0xda, 0x47);
    internal static Guid GUID_POWER_SAVING_STATUS = new Guid(0xe00958c0, 0xc213, 0x4ace, 0xac, 0x77, 0xfe, 0xcc, 0xed, 0x2e, 0xee, 0xa5);
    internal static Guid GUID_ENERGY_SAVER_STATUS = new Guid(0x550e8400, 0xe29b, 0x41d4, 0xa7, 0x16, 0x44, 0x66, 0x55, 0x44, 0x00, 0x00);
    internal static Guid GUID_SESSION_DISPLAY_STATUS = new Guid(0x2b84c20e, 0xad23, 0x4ddf, 0x93, 0xdb, 0x5, 0xff, 0xbd, 0x7e, 0xfc, 0xa5);
    internal static Guid GUID_BATTERY_PERCENTAGE_REMAINING = new Guid(0xA7AD8041, 0xB45A, 0x4CAE, 0x87, 0xA3, 0xEE, 0xCB, 0xB4, 0x68, 0xA9, 0xE1);
    internal static Guid GUID_ACDC_POWER_SOURCE = new Guid(0x5D3E9A59, 0xE9D5, 0x4B00, 0xA6, 0xBD, 0xFF, 0x34, 0xFF, 0x51, 0x65, 0x48);
    internal static Guid GUID_GLOBAL_USER_PRESENCE = new Guid(0x786e8a1d, 0xb427, 0x4344, 0x92, 0x7, 0x9, 0xe7, 0xb, 0xdc, 0xbe, 0xa9);
    internal static Guid GUID_LIDSWITCH_STATE_CHANGE = new Guid(0xBA3E0F4D, 0xB817, 0x4094, 0xA2, 0xD1, 0xD5, 0x63, 0x79, 0xE6, 0xA0, 0xF3);
    internal static Guid GUID_MONITOR_POWER_ON = new Guid(0x02731015, 0x4510, 0x4526, 0x99, 0xE6, 0xE5, 0xA1, 0x7E, 0xBD, 0x1A, 0xEA);
    internal static Guid GUID_POWERSCHEME_PERSONALITY = new Guid(0x245D8541, 0x3943, 0x4422, 0xB0, 0x25, 0x13, 0xA7, 0x84, 0xF6, 0x79, 0xB7);
    internal static Guid GUID_MIN_POWER_SAVINGS = new Guid(0x8C5E7FDA, 0xE8BF, 0x4A96, 0x9A, 0x85, 0xA6, 0xE2, 0x3A, 0x8C, 0x63, 0x5C);
    internal static Guid GUID_MAX_POWER_SAVINGS = new Guid(0xA1841308, 0x3541, 0x4FAB, 0xBC, 0x81, 0xF7, 0x15, 0x56, 0xF2, 0x0B, 0x4A);
    internal static Guid GUID_TYPICAL_POWER_SAVINGS = new Guid(0x381B4222, 0xF694, 0x41F0, 0x96, 0x85, 0xFF, 0x5B, 0xB2, 0x60, 0xDF, 0x2E);

    internal const uint WTS_CONSOLE_CONNECT = 0x01;
    internal const uint WTS_CONSOLE_DISCONNECT = 0x02;
    internal const uint WTS_REMOTE_CONNECT = 0x03;
    internal const uint WTS_REMOTE_DISCONNECT = 0x04;
    internal const uint WTS_SESSION_LOGON = 0x05;
    internal const uint WTS_SESSION_LOGOFF = 0x06;
    internal const uint WTS_SESSION_LOCK = 0x07;
    internal const uint WTS_SESSION_UNLOCK = 0x08;
    internal const uint WTS_SESSION_REMOTE_CONTROL = 0x09;
    internal const uint WTS_SESSION_CREATE = 0x0a;
    internal const uint WTS_SESSION_TERMINATE = 0x0b;

    internal const int NOTIFY_FOR_THIS_SESSION = 0x00;
    internal const int NOTIFY_FOR_ALL_SESSIONS = 0x01;

    internal static readonly IntPtr NULL = IntPtr.Zero;

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    internal struct POWERBROADCAST_SETTING
    {
        public Guid PowerSetting;
        public uint DataLength;
        public byte Data;
    }

    public delegate IntPtr WindowProcedure(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct WNDCLASSEX
    {
        [MarshalAs(UnmanagedType.U4)]
        public int cbSize;
        [MarshalAs(UnmanagedType.U4)]
        public int style;
        public WindowProcedure lpfnWndProc;
        public int cbClsExtra;
        public int cbWndExtra;
        public IntPtr hInstance;
        public IntPtr hIcon;
        public IntPtr hCursor;
        public IntPtr hbrBackground;
        public string lpszMenuName;
        public string lpszClassName;
        public IntPtr hIconSm;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MSG
    {
        public IntPtr hwnd;
        public uint message;
        public IntPtr wParam;
        public IntPtr lParam;
        public UInt32 time;
        public POINT pt;
        public UInt32 lPrivate;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct POINT
    {
        public int X;
        public int Y;

        public POINT(int x, int y)
        {
            this.X = x;
            this.Y = y;
        }

        public static implicit operator System.Drawing.Point(POINT p)
        {
            return new System.Drawing.Point(p.X, p.Y);
        }

        public static implicit operator POINT(System.Drawing.Point p)
        {
            return new POINT(p.X, p.Y);
        }
    }

    [DllImport(@"User32.dll", SetLastError = true, EntryPoint = "RegisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    internal static extern IntPtr RegisterPowerSettingNotification(IntPtr hRecipient, ref Guid PowerSettingGuid, uint Flags);

    [DllImport(@"User32.dll", EntryPoint = "UnregisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    internal static extern Int32 UnregisterPowerSettingNotification(IntPtr RegistrationHandle);

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    internal static extern IntPtr GetModuleHandle(
    [MarshalAs(UnmanagedType.LPWStr)] in string lpModuleName);

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.U2)]
    internal static extern UInt16 RegisterClassEx([In] ref WNDCLASSEX lpwcx);

    [DllImport("gdi32.dll")]
    internal static extern IntPtr GetStockObject(int fnObject);

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
    internal static extern IntPtr LoadImage(IntPtr hinst, string lpszName, uint uType,　int cxDesired, int cyDesired, uint fuLoad);

    [DllImport("user32.dll")]
    internal static extern IntPtr DefWindowProc(IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam);

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    internal static extern IntPtr CreateWindowEx(
        uint dwExStyle,
        string lpClassName,
        [MarshalAs(UnmanagedType.LPStr)] string lpWindowName,
        uint dwStyle,
        int x,
        int y,
        int nWidth,
        int nHeight,
        IntPtr hWndParent,
        IntPtr hMenu,
        IntPtr hInstance,
        IntPtr lpParam
    );

    [DllImport("user32.dll")]
    internal static extern Int32 ShowWindow(IntPtr hWnd, int nCmdShow);

    [DllImport("user32.dll")]
    internal static extern Int32 UpdateWindow(IntPtr hWnd);

    [DllImport("user32.dll")]
    internal static extern int GetMessage(out MSG lpMsg, IntPtr hWnd, uint wMsgFilterMin, uint wMsgFilterMax);

    [DllImport("user32.dll")]
    internal static extern Int32 TranslateMessage([In] ref MSG lpMsg);

    [DllImport("user32.dll")]
    internal static extern IntPtr DispatchMessage([In] ref MSG lpmsg);

    [DllImport("user32.dll")]
    internal static extern void PostQuitMessage(int nExitCode);

    [DllImport("wtsapi32.dll", SetLastError = true)]
    internal static extern Int32 WTSRegisterSessionNotification(IntPtr hWnd, [MarshalAs(UnmanagedType.U4)] int dwFlags);

    [DllImport("wtsapi32.dll", SetLastError = true)]
    internal static extern Int32 WTSUnRegisterSessionNotification(IntPtr hWnd);
}
