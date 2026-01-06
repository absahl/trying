using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor;

internal struct NetworkHealthInfo
{
    // data (GB)
    private float? _dataUploaded; // total data uploaded
    private float? _dataDownloaded; // total data downloaded

    public NetworkHealthInfo(IHardware hardware)
    {
        Console.WriteLine("Gathering network health info");
        hardware.Update();
        foreach (var sensor in hardware.Sensors)
        {
            switch (sensor.SensorType)
            {
                case SensorType.Data:
                    ReadDataSensors(sensor);
                    break;
                default:
                    Console.WriteLine($"Invalid sensor type: {sensor.SensorType}");
                    break;
            }
        }
    }

    private void ReadDataSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Data Uploaded":
                _dataUploaded = sensor.Value;
                break;
            case "Data Downloaded":
                _dataDownloaded = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid data sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    public readonly void Display()
    {
        Console.WriteLine();
        Console.WriteLine("----- Network health info <Start> -----");
        Console.WriteLine("Data uploaded: {0}GB", _dataUploaded.HasValue ? _dataUploaded.Value : "");
        Console.WriteLine("Data downloaded: {0}GB", _dataDownloaded.HasValue ? _dataDownloaded.Value : "");
        Console.WriteLine("----- Network health info <&End&> -----");
        Console.WriteLine();
    }
}
