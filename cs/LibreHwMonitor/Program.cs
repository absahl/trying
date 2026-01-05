using LibreHwMonitor;

class Program
{
    static void Main(string[] args)
    {
        var systemHealthInfo = new SystemHealthInfo { };
        systemHealthInfo.Display();
    }
}
