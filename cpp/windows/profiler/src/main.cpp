#include <windows.h>
#include <psapi.h>
#include <iphlpapi.h>
#include <iostream>

int main() {
    // --- CPU before ---
    ULONG64 cpuStart = 0;
    QueryProcessCycleTime(GetCurrentProcess(), &cpuStart);

    // --- Memory before ---
    PROCESS_MEMORY_COUNTERS_EX memStart{};
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memStart, sizeof(memStart));

    // --- Network before ---
    DWORD size = 0;
    GetIfTable(nullptr, &size, false);
    MIB_IFTABLE* ifTableStart = (MIB_IFTABLE*)malloc(size);
    GetIfTable(ifTableStart, &size, false);
    MIB_IFROW netStart{};
    if (ifTableStart->dwNumEntries > 0) {
        netStart = ifTableStart->table[0]; // first adapter
    }

    // --- Code block to profile ---
    std::cout << "Hello World!" << std::endl;

    // --- CPU after ---
    ULONG64 cpuEnd = 0;
    QueryProcessCycleTime(GetCurrentProcess(), &cpuEnd);

    // --- Memory after ---
    PROCESS_MEMORY_COUNTERS_EX memEnd{};
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memEnd, sizeof(memEnd));

    // --- Network after ---
    DWORD sizeAfter = 0;
    GetIfTable(nullptr, &sizeAfter, false);
    MIB_IFTABLE* ifTableEnd = (MIB_IFTABLE*)malloc(sizeAfter);
    GetIfTable(ifTableEnd, &sizeAfter, false);
    MIB_IFROW netEnd{};
    if (ifTableEnd->dwNumEntries > 0) {
        netEnd = ifTableEnd->table[0]; // first adapter
    }

    // --- Report deltas ---
    std::cout << "CPU cycles used: " << (cpuEnd - cpuStart) << std::endl;
    std::cout << "Memory delta (bytes): " << (memEnd.WorkingSetSize - memStart.WorkingSetSize) << std::endl;
    std::cout << "Network sent delta (bytes): " << (netEnd.dwOutOctets - netStart.dwOutOctets) << std::endl;
    std::cout << "Network received delta (bytes): " << (netEnd.dwInOctets - netStart.dwInOctets) << std::endl;

    // --- Cleanup ---
    free(ifTableStart);
    free(ifTableEnd);

    return 0;
}
