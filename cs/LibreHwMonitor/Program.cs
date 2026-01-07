using LibreHwMonitor;

class Program
{
    static void Main(string[] args)
    {
        using (var systemHealthInfo = new SystemHealthInfo { })
        {
            systemHealthInfo.Display();
        }
    }
}
