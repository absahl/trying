#include <format>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

[[noreturn]] void NoReturnFun() noexcept
{
    while (true)
    {
        std::cout << std::format("Allah Hu") << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

[[deprecated("Deprecated. Use NewFun instead")]] void OldFun() noexcept
{
    std::cout << std::format("This is a deprecated function") << std::endl;
}

[[nodiscard]] bool NoDiscardFun() noexcept
{
    std::cout << std::format("This function's return value should not be discarded") << std::endl;
    return true;
}

struct Empty
{
};

struct MyStruct
{
    int i;
    [[no_unique_address]] Empty e;
};

int main()
{
    std::cout << std::format("size of MyStruct: {}", sizeof(MyStruct)) << std::endl;
    std::cout << std::format("size of Empty: {}", sizeof(Empty)) << std::endl;
    return 0;
}
