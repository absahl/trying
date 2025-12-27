#include <windows.h>
#include <batclass.h>
#include <iostream>

int main() {
    HANDLE hBattery = CreateFile(L"\\\\.\\Battery0", GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if (hBattery == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open battery device\n";
        return 1;
    }

    BATTERY_QUERY_INFORMATION bqi{;
    BATTERY_INFORMATION info;
    DWORD bytes;
    if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION,
        &BATTERY_QUERY_INFORMATION{ BatteryInformation, 0, 0 },
        sizeof(BATTERY_QUERY_INFORMATION),
        &info, sizeof(info),
        &bytes, NULL)) {
        std::cout << "Designed Capacity: " << info.DesignedCapacity << "\n";
        std::cout << "Full Charged Capacity: " << info.FullChargedCapacity << "\n";
    }
    else {
        std::cout << "Query failed\n";
    }

    CloseHandle(hBattery);
    return 0;
}
