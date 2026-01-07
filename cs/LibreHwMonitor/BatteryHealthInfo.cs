using LibreHardwareMonitor.Hardware;

namespace LibreHwMonitor;

internal struct BatteryHealthInfo
{
    // metadata
    private readonly IHardware _hardware;

    // power (W)
    private float? _chargeRate;
    private float? _dischargeRate;

    // level (%)
    private float? _chargeLevel;
    private float? _degradationLevel;

    // capacity (mWh)
    private float? _designedCapacity;
    private float? _fullChargeCapacity;

    public BatteryHealthInfo(IHardware hardware)
    {
        _hardware = hardware;
    }

    private void ReadSensors()
    {
        Console.WriteLine($"Reading hardware sensors <{_hardware.Name}>");
        _hardware.Update();
        foreach (var sensor in _hardware.Sensors)
        {
            switch (sensor.SensorType)
            {
                case SensorType.Power:
                    ReadPowerSensors(sensor);
                    break;
                case SensorType.Level:
                    ReadLevelSensors(sensor);
                    break;
                case SensorType.Energy:
                    ReadEnergySensors(sensor);
                    break;
                default:
                    Console.WriteLine($"Invalid sensor type: {sensor.SensorType}");
                    break;
            }
        }
    }

    private void ReadPowerSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Charge Rate":
                _chargeRate = sensor.Value;
                break;
            case "Discharge Rate":
                _dischargeRate = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid power sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadLevelSensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Charge Level":
                _chargeLevel = sensor.Value;
                break;
            case "Degradation Level":
                _degradationLevel = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid level sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    private void ReadEnergySensors(ISensor sensor)
    {
        switch (sensor.Name)
        {
            case "Designed Capacity":
                _designedCapacity = sensor.Value;
                break;
            case "Fully-Charged Capacity":
                _fullChargeCapacity = sensor.Value;
                break;
            default:
                Console.WriteLine($"Invalid energy sensor: {sensor.Name} ({sensor.Value})");
                break;
        }
    }

    public void Display()
    {
        ReadSensors();
        Console.WriteLine();
        Console.WriteLine("----- Battery health info <Start> -----");
        Console.WriteLine("Hardware name: {0}", _hardware.Name);
        Console.WriteLine("Charge rate: {0}W", _chargeRate.HasValue ? _chargeRate.Value : "");
        Console.WriteLine("Discharge rate: {0}W", _dischargeRate.HasValue ? _dischargeRate.Value : "");
        Console.WriteLine("Charge level: {0}%", _chargeLevel.HasValue ? _chargeLevel.Value : "");
        Console.WriteLine("Degradation level: {0}%", _degradationLevel.HasValue ? _degradationLevel.Value : "");
        Console.WriteLine("Designed capacity: {0}mWh", _designedCapacity.HasValue ? _designedCapacity.Value : "");
        Console.WriteLine("Full charge capacity: {0}mWh", _fullChargeCapacity.HasValue ? _fullChargeCapacity.Value : "");
        Console.WriteLine("----- Battery health info <&End&> -----");
        Console.WriteLine();
    }
}
