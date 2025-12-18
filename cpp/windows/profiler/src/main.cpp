#include <thread>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <iphlpapi.h>
#include <iostream>
#include <iomanip>
#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

struct SystemTime final
{
    uint64_t mIdleTime{};
    uint64_t mKernelTime{}; // includes idle time as well
    uint64_t mUserTime{};
    bool mIsInitialized{ false };

    SystemTime() noexcept
    {
        FILETIME idleTime{};
        FILETIME kernelTime{};
        FILETIME userTime{};

        spdlog::debug("Getting aggregated system time over all cores");
        if (GetSystemTimes(&idleTime, &kernelTime, &userTime) == 0)
        {
            spdlog::error("Failed to get aggregated system time over all cores [{}]", GetLastError());
            return;
        }

        mIdleTime = static_cast<uint64_t>(idleTime.dwHighDateTime) << 32 | idleTime.dwLowDateTime;
        mKernelTime = static_cast<uint64_t>(kernelTime.dwHighDateTime) << 32 | kernelTime.dwLowDateTime;
        mUserTime = static_cast<uint64_t>(userTime.dwHighDateTime) << 32 | userTime.dwLowDateTime;
        mIsInitialized = true;
    }

    SystemTime(const uint64_t idleTime, const uint64_t kernelTime, const uint64_t userTime) noexcept : mIdleTime(idleTime), mKernelTime(kernelTime), mUserTime(userTime)
    {
    }

    bool operator!() const noexcept
    {
        return !mIsInitialized;
    }

    SystemTime operator-(const SystemTime& st) const noexcept
    {
        return SystemTime{mIdleTime - st.mIdleTime, mKernelTime - st.mKernelTime, mUserTime - st.mUserTime};
    }

    void print() const noexcept
    {
        spdlog::debug("SystemTime - idle [{}], kernel [{}], realKernel [{}], user [{}]", mIdleTime, mKernelTime, mKernelTime - mIdleTime, mUserTime);
    }

    uint64_t total() const noexcept
    {
        return mKernelTime + mUserTime;
    }

    double seconds() const noexcept
    {
        return (static_cast<double>(mKernelTime) + mUserTime) / 1e7;
    }
};

struct ProcessTime final
{
    uint64_t mKernelTime{};
    uint64_t mUserTime{};
    bool mIsInitialized{ false };

    ProcessTime() noexcept
    {
        FILETIME creationTime{};
        FILETIME exitTime{};
        FILETIME kernelTime{};
        FILETIME userTime{};

        spdlog::debug("Getting process time over all cores");
        if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime) == 0)
        {
            spdlog::error("Failed to get process time over all cores [{}]", GetLastError());
            return;
        }

        mKernelTime = static_cast<uint64_t>(kernelTime.dwHighDateTime) << 32 | kernelTime.dwLowDateTime;
        mUserTime = static_cast<uint64_t>(userTime.dwHighDateTime) << 32 | userTime.dwLowDateTime;
        mIsInitialized = true;
    }

    ProcessTime(const uint64_t kernelTime, const uint64_t userTime) noexcept : mKernelTime(kernelTime), mUserTime(userTime)
    {
    }

    bool operator!() const noexcept
    {
        return !mIsInitialized;
    }

    ProcessTime operator-(const ProcessTime& pt) const noexcept
    {
        return ProcessTime{ mKernelTime - pt.mKernelTime, mUserTime - pt.mUserTime };
    }

    void print() const noexcept
    {
        spdlog::debug("ProcessTime - kernel [{}], user [{}]", mKernelTime, mUserTime);
    }

    uint64_t total() const noexcept
    {
        return mKernelTime + mUserTime;
    }

    double seconds() const noexcept
    {
        return (static_cast<double>(mKernelTime) + mUserTime) / 1e7;
    }
};

class Profiler final
{
public:
    Profiler(const std::string& name) noexcept : mName(name)
    {
    }

    ~Profiler() noexcept
    {
        SystemTime systemTimeAtEnd{};
        ProcessTime processTimeAtEnd{};
        if (!systemTimeAtEnd || !processTimeAtEnd || !mSystemTimeAtStart || !mProcessTimeAtStart)
        {
            spdlog::error("[PROFILER:{}] Failed to compute cpu load as objects failed to initialize", mName);
            return;
        }

        // difference
        const auto systemTimeDiff = systemTimeAtEnd - mSystemTimeAtStart;
        const auto processTimeDiff = processTimeAtEnd - mProcessTimeAtStart;

        // compute load
        const auto cpuLoad = (static_cast<double>(processTimeDiff.total()) / systemTimeDiff.total()) * 100;
        spdlog::debug("[PROFILER:{}] cpu load: {:.2f}", mName, cpuLoad);

        // compute peak memory usage
        GetProcessMemoryInfo();
    }
private:
    SystemTime mSystemTimeAtStart{};
    ProcessTime mProcessTimeAtStart{};
    std::string mName{};
};

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::debug("Allah Hu");

    // code block
    {
        Profiler profiler{"Testing"};
        volatile int x = 0;
        for (int i = 0; i < 200'000'000; i++) x += i;
    }

    return 0;
}
