#include <windows.h>   // for windows types like `hInstance`
#include <vector>
#include <format>
#include <memory>
#include <iostream>
#include <setupapi.h>  // for device information set API `SetupDiGetClassDevs`
#include <initguid.h>  // for GUID definitions otherwise get link error related to `GUID_DEVICE_BATTERY`
#include <batclass.h>  // for battery device `GUID_DEVICE_BATTERY`

int main()
{
    std::wcout << std::format(L"May Allah help us find the current full charge capacity of the battery device!") << std::endl;

    // get handle to the device information set that contains all the installed devices
    HDEVINFO hDev = SetupDiGetClassDevs(
        &GUID_DEVICE_BATTERY,
        NULL, NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDev == INVALID_HANDLE_VALUE)
    {
        std::wcerr << std::format(L"Failed to get device information set <error:{}>", GetLastError()) << std::endl;
        return 1;
    }

    // device interface definition inside device information set
    SP_DEVICE_INTERFACE_DATA ifData = {};
    ifData.cbSize = sizeof(ifData);

    // enumerate battery device interfaces inside the device information set
    if (!SetupDiEnumDeviceInterfaces(hDev, nullptr, &GUID_DEVICE_BATTERY, 0, &ifData))
    {
        std::wcerr << std::format(L"Failed to enumerate battery device interfaces <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }

    // get buffer size required to store details about the battery device interface
    DWORD detailDataSize{};
    if (!SetupDiGetDeviceInterfaceDetail(hDev, &ifData, nullptr, 0, &detailDataSize, nullptr) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::wcerr << std::format(L"Failed to get size details about the device interface <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }

    // prepare the buffer to store interface details
    std::vector<uint8_t> detailVector(detailDataSize);
    auto detailData{reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(detailVector.data())};

    // fetch the interface details
    detailData->cbSize = sizeof(*detailData);
    if (!SetupDiGetDeviceInterfaceDetail(hDev, &ifData, detailData, detailDataSize, nullptr, nullptr))
    {
        std::wcerr << std::format(L"Failed to get details about the device interface <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device path <{}>", detailData->DevicePath) << std::endl;

    // open the device
    HANDLE hFile = CreateFile(
        detailData->DevicePath,
        GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::wcerr << std::format(L"Failed to open the device <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device opened <{}>", detailData->DevicePath) << std::endl;

    // query battery tag from the opened device
    ULONG batteryTag{};
    if (!DeviceIoControl(hFile, IOCTL_BATTERY_QUERY_TAG, nullptr, 0, &batteryTag, sizeof(batteryTag), nullptr, nullptr))
    {
        std::wcerr << std::format(L"Failed to query battery device for tag <{}> <error:{}>", detailData->DevicePath, GetLastError()) << std::endl;
        CloseHandle(hFile); // close the device handle
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device tag <{}>", batteryTag) << std::endl;

    // query battery information using the battery tag
    BATTERY_QUERY_INFORMATION bqi{};
    bqi.InformationLevel = BatteryInformation;
    bqi.AtRate = 0;
    bqi.BatteryTag = batteryTag;

    BATTERY_INFORMATION info{};
    if (!DeviceIoControl(hFile, IOCTL_BATTERY_QUERY_INFORMATION, &bqi, sizeof(bqi), &info, sizeof(info), nullptr, nullptr))
    {
        std::wcerr << std::format(L"Failed to query battery for info <tag:{}> <error:{}>", batteryTag, GetLastError()) << std::endl;
        CloseHandle(hFile); // close the device handle
        SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
        return 1;
    }

    // print the battery information
    std::wcout << std::format(L"Current full charge capacity <{} / {}>", info.FullChargedCapacity, info.DesignedCapacity) << std::endl;

    CloseHandle(hFile); // close the device handle
    SetupDiDestroyDeviceInfoList(hDev); // relase the device information set handle back to kernel
    return 0;
}
