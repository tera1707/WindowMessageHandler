using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;

namespace WindowJikkenCs.Model;

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

internal class NativeMethods
{
    internal const BOOL FALSE = 0;
    internal const BOOL TRUE = 1;

    // Window Class Styles
    internal const int CS_VREDRAW = 0x0001;
    internal const int CS_HREDRAW = 0x0002;

    // LoadImageのパラメータ
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

    // 拡張じゃないウィンドウスタイル
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
    internal const int PBT_POWERSETTINGCHANGE = 0x8013;
    internal static Guid GUID_CONSOLE_DISPLAY_STATE = new Guid(0x6fe69556, 0x704a, 0x47a0, 0x8f, 0x24, 0xc2, 0x8d, 0x93, 0x6f, 0xda, 0x47);
    internal const int DEVICE_NOTIFY_WINDOW_HANDLE = 0x00000000;

    internal static readonly IntPtr NULL = IntPtr.Zero;


    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    internal struct POWERBROADCAST_SETTING
    {
        public Guid PowerSetting;
        public uint DataLength;
        public byte Data;
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
    [DllImport(@"User32.dll", SetLastError = true, EntryPoint = "RegisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    internal static extern IntPtr RegisterPowerSettingNotification(IntPtr hRecipient, ref Guid PowerSettingGuid, uint Flags);

    [DllImport(@"User32.dll", EntryPoint = "UnregisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
    internal static extern bool UnregisterPowerSettingNotification(IntPtr RegistrationHandle);

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    internal static extern HINSTANCE GetModuleHandle(
    [MarshalAs(UnmanagedType.LPWStr)] in string lpModuleName);

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.U2)]
    internal static extern ATOM RegisterClassEx(
       [In] ref WNDCLASSEX lpwcx
    );

    [DllImport("gdi32.dll")]
    internal static extern HBRUSH GetStockObject(
       int fnObject
    );

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
    internal static extern HICON LoadImage(
       HINSTANCE hinst,
       string lpszName,
       uint uType,
       int cxDesired,
       int cyDesired,
       uint fuLoad
    );

    [DllImport("user32.dll")]
    internal static extern LRESULT DefWindowProc(
       HWND hWnd,
       uint uMsg,
       WPARAM wParam,
       LPARAM lParam
    );

    [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
    internal static extern HWND CreateWindowEx(
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
    internal static extern bool ShowWindow(
       HWND hWnd,
       int nCmdShow
    );

    [DllImport("user32.dll")]
    internal static extern bool UpdateWindow(
       HWND hWnd
    );

    [DllImport("user32.dll")]
    internal static extern int GetMessage(
       out MSG lpMsg,
       HWND hWnd,
       uint wMsgFilterMin,
       uint wMsgFilterMax
    );

    [DllImport("user32.dll")]
    internal static extern bool TranslateMessage(
       [In] ref MSG lpMsg
    );

    [DllImport("user32.dll")]
    internal static extern IntPtr DispatchMessage(
       [In] ref MSG lpmsg
    );

    [DllImport("user32.dll")]
    internal static extern void PostQuitMessage(
       int nExitCode
    );
}
