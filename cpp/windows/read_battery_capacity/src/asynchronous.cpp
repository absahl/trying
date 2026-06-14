#include <windows.h>   // for windows types like `hInstance`
#include <vector>
#include <format>
#include <chrono>
#include <iostream>
#include <setupapi.h>  // for device information set API `SetupDiGetClassDevs`
#include <initguid.h>  // for GUID definitions otherwise get link error related to `GUID_DEVICE_BATTERY`
#include <batclass.h>  // for battery device `GUID_DEVICE_BATTERY`

using namespace std::chrono_literals;

namespace ioctlutils
{
    enum class IoctlResult
    {
        SUCCESS,
        FAILURE,
        TIMEOUT
    };

    IoctlResult DeviceIoControlAsync(HANDLE handle, const DWORD controlCode, void* inBuffer, const DWORD inBufferSize, void* outBuffer, const DWORD outBufferSize)
    {
        OVERLAPPED overlapped{};
        overlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        if (overlapped.hEvent == nullptr)
        {
            std::wcerr << std::format(L"Failed to create a new event object for query <{}> <error:{}>", controlCode, GetLastError()) << std::endl;
            return IoctlResult::FAILURE;
        }

        if (!DeviceIoControl(handle, controlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, nullptr, &overlapped))
        {
            if (GetLastError() != ERROR_IO_PENDING)
            {
                std::wcerr << std::format(L"Failed to query device <code:{}> <error:{}>", controlCode, GetLastError()) << std::endl;
                CloseHandle(overlapped.hEvent);
                return IoctlResult::FAILURE;
            }

            // wait for the asynchronous operation to complete
            const auto timeout{5000ms.count()};
            std::wcout << std::format(L"Wait for the asynchronous operation to complete <code:{}> <timeout:{} ms>", controlCode, timeout) << std::endl;
            if (const auto waitResult = WaitForSingleObject(overlapped.hEvent, timeout); waitResult == WAIT_TIMEOUT)
            {
                std::wcerr << std::format(L"Asynchronous operation failed to complete in time <code:{}> <timeout:{} ms>", controlCode, timeout) << std::endl;
                // submit cancellation request and forget i.e. don't wait on it
                // let the caller handle it by not closing the file handle as it may block as well
                CancelIoEx(handle, &overlapped);
                CloseHandle(overlapped.hEvent);
                return IoctlResult::TIMEOUT;
            }
            else if (waitResult != WAIT_OBJECT_0)
            {
                std::wcerr << std::format(L"Waiting for asynchronous operation to complete failed <code:{}> <timeout:{} ms> <result:{}> <error:{}>", controlCode, timeout, waitResult, GetLastError()) << std::endl;
                CloseHandle(overlapped.hEvent);
                return IoctlResult::FAILURE;
            }

            // get the result of the asynchronous operation
            DWORD bytesTransferred{};
            if (!GetOverlappedResult(handle, &overlapped, &bytesTransferred, FALSE))
            {
                std::wcerr << std::format(L"Failed to get result for the asynchronous operation <error:{}>", GetLastError()) << std::endl;
                CloseHandle(overlapped.hEvent);
                return IoctlResult::FAILURE;
            }
        }

        CloseHandle(overlapped.hEvent);
        return IoctlResult::SUCCESS;
    }
}

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
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
        return 1;
    }

    // get buffer size required to store details about the battery device interface
    DWORD detailDataSize{};
    if (!SetupDiGetDeviceInterfaceDetail(hDev, &ifData, nullptr, 0, &detailDataSize, nullptr) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::wcerr << std::format(L"Failed to get size details about the device interface <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
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
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device path <{}>", detailData->DevicePath) << std::endl;

    // open the device
    HANDLE hFile = CreateFile(
        detailData->DevicePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::wcerr << std::format(L"Failed to open the device <error:{}>", GetLastError()) << std::endl;
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device opened <{}>", detailData->DevicePath) << std::endl;

    // query battery tag
    ULONG batteryTag{};
    if (const auto result = ioctlutils::DeviceIoControlAsync(hFile, IOCTL_BATTERY_QUERY_TAG, nullptr, 0, &batteryTag, sizeof(batteryTag)); result != ioctlutils::IoctlResult::SUCCESS)
    {
        // do not close the device handle if ioctl timed out as it may block as well
        if (result != ioctlutils::IoctlResult::TIMEOUT)
        {
            CloseHandle(hFile);
        }
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
        return 1;
    }
    std::wcout << std::format(L"Battery device tag <{}>", batteryTag) << std::endl;

    // query battery information using the battery tag
    BATTERY_QUERY_INFORMATION bqi{};
    bqi.InformationLevel = BatteryInformation;
    bqi.AtRate = 0;
    bqi.BatteryTag = batteryTag;

    BATTERY_INFORMATION info{};
    if (const auto result = ioctlutils::DeviceIoControlAsync(hFile, IOCTL_BATTERY_QUERY_INFORMATION, &bqi, sizeof(bqi), &info, sizeof(info)); result != ioctlutils::IoctlResult::SUCCESS)
    {
        // do not close the device handle if ioctl timed out as it may block as well
        if (result != ioctlutils::IoctlResult::TIMEOUT)
        {
            CloseHandle(hFile);
        }
        SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
        return 1;
    }

    // print the battery information
    std::wcout << std::format(L"Current full charge capacity <{} / {}>", info.FullChargedCapacity, info.DesignedCapacity) << std::endl;

    CloseHandle(hFile); // close the device handle
    SetupDiDestroyDeviceInfoList(hDev); // release the device information set handle back to kernel
    return 0;
}
