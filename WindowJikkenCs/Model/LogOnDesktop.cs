using System.Diagnostics;
using System.IO;

namespace WindowJikkenCs.Model;

internal static class LogOnDesktop
{
    private static object lockObj = new object();
    private static string logPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + @"\mylogCs.log";

    public static void WriteLine(string line)
    {
        lock (lockObj)
        {
            var now = DateTime.Now.ToString("HH:mm:ss.fff");
            var thid = Thread.CurrentThread.ManagedThreadId;

            var log = logPath + $"{now} {thid} {line}";
            Debug.WriteLine(log);
            File.AppendAllText(logPath, log + Environment.NewLine);
        }
    }
}
