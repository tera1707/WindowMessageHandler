using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WindowJikkenCs
{
    /// <summary>
    /// WindowMessageHandlerCs.xaml の相互作用ロジック
    /// </summary>
    public partial class WindowMessageHandlerCs : Window
    {
        private const int WM_CREATE = 0x0001;
        private const int WM_POWERBROADCAST = 0x0218;
        private const int PBT_POWERSETTINGCHANGE = 0x8013;
        private static Guid GUID_CONSOLE_DISPLAY_STATE = new Guid(0x6fe69556, 0x704a, 0x47a0, 0x8f, 0x24, 0xc2, 0x8d, 0x93, 0x6f, 0xda, 0x47);
        const int DEVICE_NOTIFY_WINDOW_HANDLE = 0x00000000;

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        private struct POWERBROADCAST_SETTING
        {
            public Guid PowerSetting;
            public uint DataLength;
            public byte Data;
        }

        [DllImport(@"User32.dll", SetLastError = true, EntryPoint = "RegisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr RegisterPowerSettingNotification(IntPtr hRecipient, ref Guid PowerSettingGuid, uint Flags);

        [DllImport(@"User32.dll", EntryPoint = "UnregisterPowerSettingNotification", CallingConvention = CallingConvention.StdCall)]
        static extern bool UnregisterPowerSettingNotification(IntPtr RegistrationHandle);

        private IntPtr registerConsoleDisplayHandle = IntPtr.Zero;

        public WindowMessageHandlerCs()
        {
            InitializeComponent();

            // フックの設定
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var hWnd = new WindowInteropHelper(this).EnsureHandle();
            HwndSource source = HwndSource.FromHwnd(hWnd);
            source.AddHook(new HwndSourceHook(WndProc));

            registerConsoleDisplayHandle = RegisterPowerSettingNotification(hWnd, ref GUID_CONSOLE_DISPLAY_STATE, DEVICE_NOTIFY_WINDOW_HANDLE);

        }

        private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {

            switch (msg) 
            {
                case WM_CREATE:
                    Debug.WriteLine("WM_CREATE");
                    break;
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
                
            }
            return IntPtr.Zero;
        }

    }
}
