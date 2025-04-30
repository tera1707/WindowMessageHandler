using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows;
using WindowJikkenCs.Model;
using static WindowJikkenCs.Model.NativeMethods;

namespace WindowJikkenCs;


// C#だけどウインドウをwin32APIでつくる
// https://qiita.com/ikuzak/items/6feba393150b9fbec708

// ufcpp 関数ポインター
// https://ufcpp.net/study/csharp/interop/functionpointer/

public partial class App : Application
{

    private void Application_Startup(object sender, StartupEventArgs e)
    {
        var w = new WindowMessageHandlerCs();

        w.RegisterFunction(0x8001, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            Debug.WriteLine("0x8001を受信しました。アプリを終了します。");
            Environment.Exit(0);
        });

        w.RegisterFunction(NativeMethods.WM_POWERBROADCAST, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            switch (wp.ToInt32())
            {
                case PBT_POWERSETTINGCHANGE:
                    var pbs = (POWERBROADCAST_SETTING)Marshal.PtrToStructure(lp, typeof(POWERBROADCAST_SETTING));
                    if (pbs.PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
                    {
                        if (pbs.Data == 0) Debug.WriteLine("--Display OFF");
                        else Debug.WriteLine("--Display ON");
                    }
                    break;
            }

        });

        w.CreateSpecifiedTitleClassWindow();
        Environment.Exit(0);
    }
}
