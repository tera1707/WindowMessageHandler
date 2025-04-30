using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static WindowJikkenCs.Model.NativeMethods;

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

namespace WindowJikkenCs.Model;

internal class WindowMessageHandlerCs
{
    string szAppName = "MyWindowClassName";
    string szWindowTitle = "MyWindowTitle";
    internal IntPtr registerConsoleDisplayHandle = IntPtr.Zero;

    public WindowMessageHandlerCs()
    {

    }

    public void CreateSpecifiedTitleClassWindow()
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

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    private Dictionary<uint, Action<HWND, uint, WPARAM, LPARAM>> _actinDic = new();

    public void RegisterFunction(uint msg, Action<HWND, uint, WPARAM, LPARAM> actinDic)
    {
        _actinDic.Add(msg, actinDic);
    }

    // コールバック
    private LRESULT WndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam)
    {
        if (_actinDic.TryGetValue(msg, out var action))
            action.Invoke(hWnd, msg, wParam, lParam);

        switch (msg)
        {
            case WM_CREATE:
                registerConsoleDisplayHandle = RegisterPowerSettingNotification(hWnd, ref GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
        }

        return NULL;
    }
}
