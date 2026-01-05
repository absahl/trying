using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor
{
    internal struct MemoryHealthInfo
    {
        // load (%)
        private float? _physicalMemoryUsage;
        private float? _totalMemoryUsage; // ram + swap + reserved

        // data (GB)
        private float? _physicalMemoryUsed;
        private float? _physicalMemoryAvailable;
        private float? _totalMemoryUsed;
        private float? _totalMemoryAvailable;
        public MemoryHealthInfo(IHardware hardware)
        {
            Console.WriteLine("Gathering memory health info");
            hardware.Update();
            foreach (var sensor in hardware.Sensors)
            {
                switch (sensor.SensorType)
                {
                    case SensorType.Load:
                        ReadLoadSensors(sensor);
                        break;
                    case SensorType.Data:
                        ReadDataSensors(sensor);
                        break;
                    default:
                        Console.WriteLine($"Invalid sensor type: {sensor.SensorType}");
                        break;
                }
            }
        }

        private void ReadLoadSensors(ISensor sensor)
        {
            switch (sensor.Name)
            {
                case "Memory":
                    _physicalMemoryUsage = sensor.Value;
                    break;
                case "Virtual Memory":
                    _totalMemoryUsage = sensor.Value;
                    break;
                default:
                    Console.WriteLine($"Invalid load sensor: {sensor.Name} ({sensor.Value})");
                    break;
            }
        }

        private void ReadDataSensors(ISensor sensor)
        {
            switch (sensor.Name)
            {
                case "Memory Used":
                    _physicalMemoryUsed = sensor.Value;
                    break;
                case "Memory Available":
                    _physicalMemoryAvailable = sensor.Value;
                    break;
                case "Virtual Memory Used":
                    _totalMemoryUsed = sensor.Value;
                    break;
                case "Virtual Memory Available":
                    _totalMemoryAvailable = sensor.Value;
                    break;
                default:
                    Console.WriteLine($"Invalid data sensor: {sensor.Name} ({sensor.Value})");
                    break;
            }
        }

        public readonly void Display()
        {
            Console.WriteLine();
            Console.WriteLine("----- Memory health info <Start> -----");
            Console.WriteLine("Physical memory usage: {0}%", _physicalMemoryUsage.HasValue ? _physicalMemoryUsage.Value : "");
            Console.WriteLine("Total memory usage: {0}%", _totalMemoryUsage.HasValue ? _totalMemoryUsage.Value : "");
            Console.WriteLine("Physical memory used: {0}GB", _physicalMemoryUsed.HasValue ? _physicalMemoryUsed.Value : "");
            Console.WriteLine("Physical memory available: {0}GB", _physicalMemoryAvailable.HasValue ? _physicalMemoryAvailable.Value : "");
            Console.WriteLine("Total memory used: {0}GB", _totalMemoryUsed.HasValue ? _totalMemoryUsed.Value : "");
            Console.WriteLine("Total memory available: {0}GB", _totalMemoryAvailable.HasValue ? _totalMemoryAvailable.Value : "");
            Console.WriteLine("----- Memory health info <&End&> -----");
            Console.WriteLine();
        }
    }
}
