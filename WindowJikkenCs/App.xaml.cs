using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq.Expressions;
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
    Dictionary<Guid, string> powerSettingTbl = new ()
    {
        { GUID_CONSOLE_DISPLAY_STATE, "GUID_CONSOLE_DISPLAY_STATE"},
        { GUID_SESSION_DISPLAY_STATUS, "GUID_SESSION_DISPLAY_STATUS"},
        { GUID_BATTERY_PERCENTAGE_REMAINING, "GUID_BATTERY_PERCENTAGE_REMAINING"},
        { GUID_ACDC_POWER_SOURCE, "GUID_ACDC_POWER_SOURCE"},
        { GUID_GLOBAL_USER_PRESENCE, "GUID_GLOBAL_USER_PRESENCE"},
        { GUID_LIDSWITCH_STATE_CHANGE, "GUID_LIDSWITCH_STATE_CHANGE"},
        { GUID_MONITOR_POWER_ON, "GUID_MONITOR_POWER_ON"},
        { GUID_POWER_SAVING_STATUS, "GUID_POWER_SAVING_STATUS"},
        { GUID_ENERGY_SAVER_STATUS, "GUID_ENERGY_SAVER_STATUS"},
        { GUID_POWERSCHEME_PERSONALITY, "GUID_POWERSCHEME_PERSONALITY"},
        { GUID_MIN_POWER_SAVINGS, "GUID_MIN_POWER_SAVINGS"},
        { GUID_MAX_POWER_SAVINGS, "GUID_MAX_POWER_SAVINGS"},
        { GUID_TYPICAL_POWER_SAVINGS, "GUID_TYPICAL_POWER_SAVINGS"},
    };

    Dictionary<uint, string> sessionEventTbl = new() 
    { 
        { WTS_CONSOLE_CONNECT, "WTS_CONSOLE_CONNECT"},
	    { WTS_CONSOLE_DISCONNECT, "WTS_CONSOLE_DISCONNECT"},
	    { WTS_REMOTE_CONNECT, "WTS_REMOTE_CONNECT" },
	    { WTS_REMOTE_DISCONNECT, "WTS_REMOTE_DISCONNECT" },
	    { WTS_SESSION_LOGON, "WTS_SESSION_LOGON" },
	    { WTS_SESSION_LOGOFF, "WTS_SESSION_LOGOFF" },
	    { WTS_SESSION_LOCK, "WTS_SESSION_LOCK" },//https://blog.kaorun55.com/entry/20080212/1202829851
	    { WTS_SESSION_UNLOCK, "WTS_SESSION_UNLOCK" },
	    { WTS_SESSION_REMOTE_CONTROL, "WTS_SESSION_REMOTE_CONTROL" },
	    { WTS_SESSION_CREATE, "WTS_SESSION_CREATE" },
	    { WTS_SESSION_TERMINATE, "WTS_SESSION_TERMINATE" },
    };

private void Application_Startup(object sender, StartupEventArgs e)
    {
        var w = new WindowMessageHandlerCs();

        w.RegisterFunction(0x8001, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            LogOnDesktop.WriteLine("0x8001を受信しました。アプリを終了します。");
            Environment.Exit(0);
        });

        w.RegisterFunction(NativeMethods.WM_POWERBROADCAST, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            switch (wp.ToInt32())
            {
                case PBT_POWERSETTINGCHANGE:
                    var pbs = (POWERBROADCAST_SETTING)Marshal.PtrToStructure(lp, typeof(POWERBROADCAST_SETTING))!;

                    if (powerSettingTbl.ContainsKey(pbs.PowerSetting))
                    {
                        LogOnDesktop.WriteLine($"{powerSettingTbl[pbs.PowerSetting]} : {pbs.Data}");
                    }
                    break;

                case PBT_APMSUSPEND:
                    LogOnDesktop.WriteLine($"PBT_APMSUSPEND");
                    break;

                case PBT_APMRESUMESUSPEND:
                    LogOnDesktop.WriteLine($"PBT_APMRESUMESUSPEND");
                    break;

                case PBT_APMRESUMEAUTOMATIC:
                    LogOnDesktop.WriteLine($"PBT_APMRESUMEAUTOMATIC");
                    break;

                case PBT_APMBATTERYLOW:
                    LogOnDesktop.WriteLine($"PBT_APMBATTERYLOW");
                    break;

                case PBT_APMPOWERSTATUSCHANGE:
                    LogOnDesktop.WriteLine($"PBT_APMPOWERSTATUSCHANGE");
                    break;
            }
        });

        w.RegisterFunction(WM_QUERYENDSESSION, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            LogOnDesktop.WriteLine($"WM_QUERYENDSESSION");
        });

        w.RegisterFunction(WM_ENDSESSION, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            LogOnDesktop.WriteLine($"WM_ENDSESSION");
        });

        w.RegisterFunction(WM_USERCHANGED, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            LogOnDesktop.WriteLine($"WM_USERCHANGED");
        });

        w.RegisterFunction(WM_WTSSESSION_CHANGE, (IntPtr hWnd, uint msg, IntPtr wp, IntPtr lp) =>
        {
            //var w = (uint)Marshal.PtrToStructure(lp, typeof(uint))!;
            var w = (uint)wp;
            if (sessionEventTbl.ContainsKey(w))
            {
                LogOnDesktop.WriteLine($"{sessionEventTbl[w]}");
            }
        });

        w.CreateSpecifiedTitleClassWindow();
        Environment.Exit(0);
    }

    public static string GetName<T>(Expression<Func<T>> e)
    {
        var member = (MemberExpression)e.Body;
        return member.Member.Name;
    }
}
