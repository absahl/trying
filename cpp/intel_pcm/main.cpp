#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "cpucounters.h"

using namespace pcm;

int main() {
    PCM* m = PCM::getInstance();
    /*if (m->program() != PCM::Success) {
        std::cerr << "PCM couldn't start\n";
        return 1;
    }

    std::cout << "Measuring CPU usage and power over 1 second...\n";

    SystemCounterState s_before = getSystemCounterState();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    SystemCounterState s_after = getSystemCounterState();

    double cpu_usage = getSystemCpuUsage(s_before, s_after) * 100.0;
    std::cout << "CPU usage: " << cpu_usage << "%\n";

    for (int i = 0; i < m->getNumSockets(); ++i) {
        double temp = getPackageTemperature(i);
        double power = getPackageThermalSpecPower(i);
        std::cout << "Socket " << i << " - Temperature: " << temp
            << " C, Power: " << power << " W\n";
    }

    m->cleanup();*/
    return 0;
}
