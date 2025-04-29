using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows;

namespace WindowJikkenCs;


// 見た目を不穏にするためのエイリアス
using ATOM = System.UInt16;
using BOOL = System.Int32;
using DWORD = System.UInt32;
using HBRUSH = System.IntPtr;
using HCURSOR = System.IntPtr;
using HICON = System.IntPtr;
using HINSTANCE = System.IntPtr;
using HMENU = System.IntPtr;
using HWND = System.IntPtr;
using LRESULT = System.IntPtr;
using LPARAM = System.IntPtr;
using WPARAM = System.IntPtr;

// C#だけどウインドウをwin32APIでつくる
// https://qiita.com/ikuzak/items/6feba393150b9fbec708

/// <summary>
/// Interaction logic for App.xaml
/// </summary>
public partial class App : Application
{
    string szAppName = "WindowClassName";
    string szWindowTitle = "WindowTitle";
    IntPtr NULL = IntPtr.Zero;

    private void Application_Startup(object sender, StartupEventArgs e)
    {
        MSG msg;
        int bRet;

        var hCurInst = GetModuleHandle(null);

        if (InitApp(hCurInst) == 0)
            return;
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

        Environment.Exit(0);
    }


    // ウィンドウクラス登録
    ATOM InitApp(HINSTANCE hInst)
    {
        var wcx = new WNDCLASSEX()
        {
            cbSize = Marshal.SizeOf(typeof(WNDCLASSEX)),
            style = CS_VREDRAW | CS_HREDRAW,
            lpfnWndProc = (WNDPROC<WindowProcedure>)WndProc,
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
    BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
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


        MyInit(hWnd);


        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    void MyInit(HWND hWnd)
    {
        registerConsoleDisplayHandle = RegisterPowerSettingNotification(hWnd, ref GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
    }

    // コールバック
    LRESULT WndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_POWERBROADCAST:
                switch (wParam.ToInt32())
                {
                    case PBT_POWERSETTINGCHANGE:
                        var pbs = (POWERBROADCAST_SETTING)Marshal.PtrToStructure(lParam, typeof(POWERBROADCAST_SETTING));
                        if (pbs.PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
                        {
                            if (pbs.Data == 0) Debug.WriteLine("--Display OFF");
                            else Debug.WriteLine("--Display ON");
                        }
                        break;
                }

                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return NULL;
    }


    const BOOL FALSE = 0;
    const BOOL TRUE = 1;

    // Window Class Styles
    const int CS_VREDRAW = 0x0001;
    const int CS_HREDRAW = 0x0002;

    // LoadImageのパラメータ
    const int LR_DEFAULTSIZE = 0x00000040;
    const int LR_SHARED = 0x00008000;
    const int IMAGE_ICON = 1;
    const int IMAGE_CURSOR = 2;
    const string IDI_APPLICATION = "#32512";
    const string IDC_ARROW = "#32512";

    // 拡張ウィンドウスタイル
    const uint WS_EX_WINDOWEDGE = 0x00000100;
    const uint WS_EX_CLIENTEDGE = 0x00000200;
    const uint WS_EX_OVERLAPPEDWINDOW = WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE;

    // 拡張じゃないウィンドウスタイル
    const uint WS_OVERLAPPED = 0x00000000;
    const uint WS_MAXIMIZEBOX = 0x00010000;
    const uint WS_MINIMIZEBOX = 0x00020000;
    const uint WS_THICKFRAME = 0x00040000;
    const uint WS_SYSMENU = 0x00080000;
    const uint WS_CAPTION = 0x00C00000;
    const uint WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

    // その他諸々
    const int WHITE_BRUSH = 0;
    const int SW_HIDE = 0;
    const int SW_SHOWNORMAL = 1;
    const int CW_USEDEFAULT = unchecked((int)0x80000000);
    const uint WM_DESTROY = 0x0002;

    private const int WM_CREATE = 0x0001;
    private const int WM_POWERBROADCAST = 0x0218;
    private const int PBT_POWERSETTINGCHANGE = 0x8013;
    private static Guid GUID_CONSOLE_DISPLAY_STATE = new Guid(0x6fe69556, 0x704a, 0x47a0, 0x8f, 0x24, 0xc2, 0x8d, 0x93, 0x6f, 0xda, 0x47);
    const int DEVICE_NOTIFY_WINDOW_HANDLE = 0x00000000;

    [DllImport(@"User32.dll", SetLastError = true, EntryPoint = "RegisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    static extern IntPtr RegisterPowerSettingNotification(IntPtr hRecipient, ref Guid PowerSettingGuid, uint Flags);

    [DllImport(@"User32.dll", EntryPoint = "UnregisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    static extern bool UnregisterPowerSettingNotification(IntPtr RegistrationHandle);

    private IntPtr registerConsoleDisplayHandle = IntPtr.Zero;

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    private struct POWERBROADCAST_SETTING
    {
        public Guid PowerSetting;
        public uint DataLength;
        public byte Data;
    }

    //var NULL = IntPtr.Zero;
    // ウィンドウクラス登録時に情報を突っ込む
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct WNDCLASSEX
    {
        [MarshalAs(UnmanagedType.U4)]
        public int cbSize;
        [MarshalAs(UnmanagedType.U4)]
        public int style;
        public WNDPROC<WindowProcedure> lpfnWndProc;
        public int cbClsExtra;
        public int cbWndExtra;
        public HINSTANCE hInstance;
        public HICON hIcon;
        public HCURSOR hCursor;
        public HBRUSH hbrBackground;
        public string lpszMenuName;
        public string lpszClassName;
        public HICON hIconSm;
    }

    // 関数ポインタをそれっぽくするためのラッパー
    public struct WNDPROC<TDelegate>
    {
        private readonly IntPtr ptr;

        public WNDPROC(IntPtr ptr)
        {
            this.ptr = ptr;
        }

        public static explicit operator WNDPROC<TDelegate>(TDelegate @delegate)
            => new(Marshal.GetFunctionPointerForDelegate(@delegate));

        public static implicit operator IntPtr(WNDPROC<TDelegate> wndproc)
            => wndproc.ptr;
    }

    public delegate IntPtr WindowProcedure(HWND hWnd, uint msg, IntPtr wParam, IntPtr lParam);

    [StructLayout(LayoutKind.Sequential)]
    public struct MSG
    {
        public HWND hwnd;
        public uint message;
        public WPARAM wParam;
        public LPARAM lParam;
        public DWORD time;
        public POINT pt;
        public DWORD lPrivate;
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
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    static extern HINSTANCE GetModuleHandle(
   [MarshalAs(UnmanagedType.LPWStr)] in string lpModuleName
);

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.U2)]
    static extern ATOM RegisterClassEx(
       [In] ref WNDCLASSEX lpwcx
    );

    [DllImport("gdi32.dll")]
    static extern HBRUSH GetStockObject(
       int fnObject
    );

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
    static extern HICON LoadImage(
       HINSTANCE hinst,
       string lpszName,
       uint uType,
       int cxDesired,
       int cyDesired,
       uint fuLoad
    );

    [DllImport("user32.dll")]
    static extern LRESULT DefWindowProc(
       HWND hWnd,
       uint uMsg,
       WPARAM wParam,
       LPARAM lParam
    );

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    static extern HWND CreateWindowEx(
        uint dwExStyle,
        string lpClassName,
        [MarshalAs(UnmanagedType.LPStr)] string lpWindowName,
        uint dwStyle,
        int x,
        int y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        HMENU hMenu,
        HINSTANCE hInstance,
        IntPtr lpParam
    );

    [DllImport("user32.dll")]
    static extern bool ShowWindow(
       HWND hWnd,
       int nCmdShow
    );

    [DllImport("user32.dll")]
    static extern bool UpdateWindow(
       HWND hWnd
    );

    [DllImport("user32.dll")]
    static extern int GetMessage(
       out MSG lpMsg,
       HWND hWnd,
       uint wMsgFilterMin,
       uint wMsgFilterMax
    );

    [DllImport("user32.dll")]
    static extern bool TranslateMessage(
       [In] ref MSG lpMsg
    );

    [DllImport("user32.dll")]
    static extern IntPtr DispatchMessage(
       [In] ref MSG lpmsg
    );

    [DllImport("user32.dll")]
    static extern void PostQuitMessage(
       int nExitCode
    );
}
