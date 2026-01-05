using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor;

internal struct CPUHealthInfo
{
    // voltage (V)
    private float? _voltage;

    // power (W)
    private float? _packagePower;
    private float? _corePower;
    private float? _memoryPower;
    private float? _platformPower;

    // clock speed (MHz)
    private float? _busSpeed;

    // temperature (°C)
    private float? _coreMaxTemperature;
    private float? _packageTemperature;

    // load (%)
    private float? _totalLoad;

    public CPUHealthInfo(IHardware hardware)
    {
        Console.WriteLine("Gathering cpu health info");
        hardware.Update();
        foreach (var sensor in hardware.Sensors)
        {
            switch (sensor.SensorType)
            {
                case SensorType.Voltage:
                    ReadVoltageSensors(sensor);
                    break;
                case SensorType.Power:
                    ReadPowerSensors(sensor);
                    break;
                case SensorType.Clock:
                    ReadClockSensors(sensor);
                    break;
                case SensorType.Temperature:
                    ReadTemperatureSensors(sensor);
                    break;
                case SensorType.Load:
                    ReadLoadSensors(sensor);
                    break;
                default:
                    Console.WriteLine($"Invalid sensor type: {sensor.SensorType}");
                    break;
            }
        }
    }

    private void ReadVoltageSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "CPU Core":
                _voltage = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid voltage sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadPowerSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "CPU Package":
                _packagePower = sensor.Value;
                break;
            case "CPU Cores":
                _corePower = sensor.Value;
                break;
            case "CPU Memory":
                _memoryPower = sensor.Value;
                break;
            case "CPU Platform":
                _platformPower = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid power sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }
    private void ReadClockSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Bus Speed":
                _busSpeed = sensor.Value;
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
            case "Core Max":
                _coreMaxTemperature = sensor.Value;
                break;
            case "CPU Package":
                _packageTemperature = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid temperature sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadLoadSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "CPU Total":
                _totalLoad = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid load sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    public readonly void Display()
    {
        Console.WriteLine();
        Console.WriteLine("----- CPU health info <Start> -----");
        Console.WriteLine("Voltage: {0}V", _voltage.HasValue ? _voltage.Value : "");
        Console.WriteLine("Package power: {0}W", _packagePower.HasValue ? _packagePower.Value : "");
        Console.WriteLine("Core power: {0}W", _corePower.HasValue ? _corePower.Value : "");
        Console.WriteLine("Memory power: {0}W", _memoryPower.HasValue ? _memoryPower.Value : "");
        Console.WriteLine("Platform power: {0}W", _platformPower.HasValue ? _platformPower.Value : "");
        Console.WriteLine("Bus speed: {0}MHz", _busSpeed.HasValue ? _busSpeed.Value : "");
        Console.WriteLine("CPU core temperature (max): {0}°C", _coreMaxTemperature.HasValue ? _coreMaxTemperature.Value : "");
        Console.WriteLine("CPU package temperature: {0}°C", _packageTemperature.HasValue ? _packageTemperature.Value : "");
        Console.WriteLine("CPU total load: {0}%", _totalLoad.HasValue ? _totalLoad.Value : "");
        Console.WriteLine("----- CPU health info <&End&> -----");
        Console.WriteLine();
    }
}
