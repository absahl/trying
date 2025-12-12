#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <format>

constexpr auto LOCK_FILE = "file.lock";

bool disable_inheritance_on_file_handle(const int fd)
{
    std::cout << std::format("Disabling inheritance on file descriptor [{}]", fd) << std::endl;
    const auto flags = fcntl(fd, F_GETFD, 0);
    if (flags == -1)
    {
        std::cerr << std::format("Failed to get file descriptor flags for file descriptor [{}]", fd) << std::endl;
        return false;
    }
    std::cout << std::format("File descriptor flags obtained for file descriptor [{}] [flags:{}]", fd, flags) << std::endl;

    const auto new_flags = flags | FD_CLOEXEC;
    std::cout << std::format("Setting FD_CLOEXEC on file descriptor [{}] [new_flags:{}]", fd, new_flags) << std::endl;
    if (fcntl(fd, F_SETFD, new_flags) == -1)
    {
        std::cerr << std::format("Failed to set FD_CLOEXEC on file descriptor [{}] [new_flags:{}]", fd, new_flags) << std::endl;
        return false;
    }
    std::cout << std::format("FD_CLOEXEC set on file descriptor [{}] [new_flags:{}]", fd, new_flags) << std::endl;
    return true;
}

int main()
{
    auto pid = getpid(); // Get parent PID

    // Open file
    std::cout << std::format("Opening file [{}]", LOCK_FILE) << std::endl;
    const auto fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        std::cerr << std::format("PID [{}] Failed to open file [{}]", pid, LOCK_FILE) << std::endl;
        return 1;
    }
    std::cout << std::format("File opened [{}] [fd:{}]", LOCK_FILE, fd) << std::endl;

    // Set exclusive lock
    std::cout << std::format("Setting exclusive lock on file [{}] [fd:{}]", LOCK_FILE, fd) << std::endl;
    if (flock(fd, LOCK_EX | LOCK_NB) == -1)
    {
        std::cerr << std::format("PID [{}] Failed to lock file [{}]", pid, LOCK_FILE) << std::endl;
        close(fd);
        return 1;
    }
    std::cout << std::format("PID [{}] File locked [{}] [fd:{}]", pid, LOCK_FILE, fd) << std::endl;

    // std::cout << std::format("Disabling inheritance on file descriptor [{}]", fd) << std::endl;
    // if (!disable_inheritance_on_file_handle(fd))
    // {
    //     std::cerr << std::format("PID [{}] Failed to disable inheritance on file descriptor [{}]", pid, fd) << std::endl;
    //     flock(fd, LOCK_UN);
    //     close(fd);
    //     return 1;
    // }
    // std::cout << std::format("Inheritance disabled on file descriptor [{}]", fd) << std::endl;

    // Fork child
    const auto child_pid = fork();
    if (child_pid == -1)
    {
        std::cerr << std::format("PID [{}] Failed to fork", pid) << std::endl;
        flock(fd, LOCK_UN);
        close(fd);
        return 1;
    }
    else if (child_pid == 0) // child process
    {
        pid = getpid(); // Update PID for child
        std::cout << std::format("PID [{}] Child: Executing 'sleep 30'", pid) << std::endl;
        execl("/bin/sleep", "sleep", "30", nullptr);
        std::cerr << std::format("PID [{}] Child: Failed to execute 'sleep 30'", pid) << std::endl;
        exit(1);
    }

    // Parent exits
    std::cout << std::format("PID [{}] Parent: Exiting", pid) << std::endl;
    return 0;
}
