using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor
{
    internal struct GPUHealthInfo
    {
        // clock speed (MHz)
        private float? _coreClockSpeed;
        private float? _memoryClockSpeed;
        private float? _shaderClockSpeed;

        // temperature (°C)
        private float? _coreTemperature;

        // load (%)
        private float? _coreLoad;
        private float? _memoryControllerLoad;
        private float? _videoEngineLoad;
        private float? _memoryLoad;
        private float? _busLoad;

        // data (GB)
        private float? _memoryAvailable;
        private float? _memoryUsed;

        public GPUHealthInfo(IHardware hardware)
        {
            Console.WriteLine("Gathering gpu health info");
            hardware.Update();
            foreach (var sensor in hardware.Sensors)
            {
                switch (sensor.SensorType)
                {
                    case SensorType.Clock:
                        ReadClockSensors(sensor);
                        break;
                    case SensorType.Temperature:
                        ReadTemperatureSensors(sensor);
                        break;
                    default:
                        Console.WriteLine($"Invalid sensor type: {sensor.SensorType}");
                        break;
                }
            }
        }

        private void ReadClockSensors(ISensor sensor)
        {
            switch (sensor.Name)
            {
                case "GPU Core":
                    _coreClockSpeed = sensor.Value;
                    break;
                case "GPU Memory":
                    _memoryClockSpeed = sensor.Value;
                    break;
                case "GPU Shader":
                    _shaderClockSpeed = sensor.Value;
                    break;
                default:
                    Console.WriteLine($"Invalid clock sensor: {sensor.Name} ({sensor.Value})");
                    break;
            }
        }

        private void ReadTemperatureSensors(ISensor sensor)
        {
            switch (sensor.Name)
            {
                case "GPU Core":
                    _coreTemperature = sensor.Value;
                    break;
                default:
                    Console.WriteLine($"Invalid temperature sensor: {sensor.Name} ({sensor.Value})");
                    break;
            }
        }

        public readonly void Display()
        {
            Console.WriteLine();
            Console.WriteLine("----- GPU health info <Start> -----");
            Console.WriteLine("Core clock speed: {0}MHz", _coreClockSpeed.HasValue ? _coreClockSpeed.Value : "");
            Console.WriteLine("Memory clock speed: {0}MHz", _memoryClockSpeed.HasValue ? _memoryClockSpeed.Value : "");
            Console.WriteLine("Shader clock speed: {0}MHz", _shaderClockSpeed.HasValue ? _shaderClockSpeed.Value : "");
            Console.WriteLine("Core temperature: {0}°C", _coreTemperature.HasValue ? _coreTemperature.Value : "");
            Console.WriteLine("----- GPU health info <&End&> -----");
            Console.WriteLine();
        }
    }
}
