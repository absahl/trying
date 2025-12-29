#include <windows.h>
#include <wlanapi.h>
#include <iostream>

int main() {
    HANDLE hClient = nullptr;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    if (WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &hClient) != ERROR_SUCCESS) {
        std::cerr << "Failed to open WLAN handle\n";
        return 1;
    }

    PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
    if (WlanEnumInterfaces(hClient, nullptr, &pIfList) != ERROR_SUCCESS) {
        std::cerr << "Failed to enumerate interfaces\n";
        return 1;
    }

    for (auto i = 0; i < pIfList->dwNumberOfItems; ++i)
    {
        WLAN_INTERFACE_INFO ifInfo = pIfList->InterfaceInfo[i];
        std::cout << "--------------------- Interface#" << i + 1 << "--------------------" << std::endl;
        std::wcout << L"Interface: " << ifInfo.strInterfaceDescription << L"\n";

        PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = nullptr;
        DWORD connectInfoSize = 0;
        WLAN_OPCODE_VALUE_TYPE opCode;
        if (WlanQueryInterface(hClient, &ifInfo.InterfaceGuid, wlan_intf_opcode_current_connection,
            nullptr, &connectInfoSize, (PVOID*)&pConnectInfo, &opCode) == ERROR_SUCCESS) {
            std::cout << "Signal Quality: " << pConnectInfo->wlanAssociationAttributes.wlanSignalQuality << "%\n";
            //std::cout << "RSSI: " << pConnectInfo->wlanAssociationAttributes.rssi << " dBm\n";
            WlanFreeMemory(pConnectInfo);
        }

        PWLAN_BSS_LIST pBssList{ nullptr };
        if (WlanGetNetworkBssList(hClient, &ifInfo.InterfaceGuid, nullptr, dot11_BSS_type_any, false, nullptr, &pBssList) == ERROR_SUCCESS) {
            for (unsigned int j = 0; j < pBssList->dwNumberOfItems; ++j) {
                WLAN_BSS_ENTRY entry = pBssList->wlanBssEntries[j];
                std::cout << "SSID: " << std::string((char*)entry.dot11Ssid.ucSSID, entry.dot11Ssid.uSSIDLength)
                    << ", RSSI: " << entry.lRssi << " dBm\n";
            }
            WlanFreeMemory(pBssList);
        }

        std::cout << "-----------------------------------------------------" << std::endl;
    }

    WlanFreeMemory(pIfList);
    WlanCloseHandle(hClient, nullptr);
    return 0;
}
