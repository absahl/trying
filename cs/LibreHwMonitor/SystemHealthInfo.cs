using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor;

struct SystemHealthInfo
{
    private readonly Computer _computer;
    private CPUHealthInfo _cpuHealthInfo;
    private MemoryHealthInfo _memoryHealthInfo;
    private GPUHealthInfo _gpuHealthInfo;
    private StorageHealthInfo _storageHealthInfo;

    public SystemHealthInfo()
    {
        Console.WriteLine("Gathering system health info");

        _computer = new Computer
        {
            IsCpuEnabled = true,
            IsGpuEnabled = true,
            IsMemoryEnabled = true,
            //IsMotherboardEnabled = true,
            //IsControllerEnabled = true,
            //IsNetworkEnabled = true,
            IsStorageEnabled = true
        };
        _computer.Open();

        foreach (var hardware in _computer.Hardware)
        {
            switch (hardware.HardwareType)
            {
                case HardwareType.Cpu:
                    Console.WriteLine($"Found CPU: {hardware.Name}");
                    _cpuHealthInfo = new CPUHealthInfo(hardware);
                    break;
                case HardwareType.Memory:
                    Console.WriteLine($"Found Memory: {hardware.Name}");
                    _memoryHealthInfo = new MemoryHealthInfo(hardware);
                    break;
                case HardwareType.GpuNvidia:
                case HardwareType.GpuAmd:
                case HardwareType.GpuIntel:
                    Console.WriteLine($"Found GPU: {hardware.Name}");
                    _gpuHealthInfo = new GPUHealthInfo(hardware);
                    break;
                case HardwareType.Storage:
                    Console.WriteLine($"Found Storage: {hardware.Name}");
                    _storageHealthInfo = new StorageHealthInfo(hardware);
                    break;
                default:
                    Console.WriteLine($"Invalid hardware type: {hardware.HardwareType}");
                    continue;
            }
        }

        _computer.Close();
    }

    public readonly void Display()
    {
        Console.WriteLine();
        Console.WriteLine("----------- System health info <Start> -----------");
        _cpuHealthInfo.Display();
        _memoryHealthInfo.Display();
        _gpuHealthInfo.Display();
        _storageHealthInfo.Display();
        Console.WriteLine("----------- System health info <&End&> -----------");
        Console.WriteLine();
    }
}
