using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor;

internal struct StorageHealthInfo
{
    // temperature (°C)
    private float? _temperature;

    // load (%)
    private float? _usedSpace; // used storage space

    // level (%)
    private float? _availableSpare; // spare blocks/sectors available
    private float? _availableSpareThreshold; // threshold for spare blocks/sectors
    private float? _lifeUsed; // percentage of life used

    // data (GB)
    private float? _dataRead; // total data read
    private float? _dataWritten; // total data written

    public StorageHealthInfo(IHardware hardware)
    {
        Console.WriteLine("Gathering storage health info");
        hardware.Update();
        foreach (var sensor in hardware.Sensors)
        {
            switch (sensor.SensorType)
            {
                case SensorType.Temperature:
                    ReadTemperatureSensors(sensor);
                    break;
                case SensorType.Load:
                    ReadLoadSensors(sensor);
                    break;
                case SensorType.Level:
                    ReadLevelSensors(sensor);
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

    private void ReadTemperatureSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Temperature":
                _temperature = sensor.Value;
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
            case "Used Space":
                _usedSpace = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid load sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadLevelSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Available Spare":
                _availableSpare = sensor.Value;
                break;
            case "Available Spare Threshold":
                _availableSpareThreshold = sensor.Value;
                break;
            case "Percentage Used":
                _lifeUsed = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid level sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadDataSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Data Read":
                _dataRead = sensor.Value;
                break;
            case "Data Written":
                _dataWritten = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid data sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    public readonly void Display()
    {
        Console.WriteLine();
        Console.WriteLine("----- Storage health info <Start> -----");
        Console.WriteLine("Temperature: {0}°C", _temperature.HasValue ? _temperature.Value : "");
        Console.WriteLine("Used space: {0}%", _usedSpace.HasValue ? _usedSpace.Value : "");
        Console.WriteLine("Available spare: {0}%", _availableSpare.HasValue ? _availableSpare.Value : "");
        Console.WriteLine("Available spare threshold: {0}%", _availableSpareThreshold.HasValue ? _availableSpareThreshold.Value : "");
        Console.WriteLine("Life used: {0}%", _lifeUsed.HasValue ? _lifeUsed.Value : "");
        Console.WriteLine("Data Read: {0}GB", _dataRead.HasValue ? _dataRead.Value : "");
        Console.WriteLine("Data Written: {0}GB", _dataWritten.HasValue ? _dataWritten.Value : "");
        Console.WriteLine("----- Storage health info <&End&> -----");
        Console.WriteLine();
    }
}
