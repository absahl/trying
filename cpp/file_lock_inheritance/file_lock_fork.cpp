#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    pid_t pid = getpid(); // Get parent PID
    // Open file
    int fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        std::cerr << "PID " << pid << ": Failed to open file" << std::endl;
        return 1;
    }

    // Set exclusive lock
    if (flock(fd, LOCK_EX) == -1) {
        std::cerr << "PID " << pid << ": Failed to lock file" << std::endl;
        close(fd);
        return 1;
    }
    std::cout << "PID " << pid << ": Parent: File locked" << std::endl;

    // Prevent descriptor inheritance
    if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
        std::cerr << "PID " << pid << ": Failed to set FD_CLOEXEC" << std::endl;
        close(fd);
        return 1;
    }

    // Fork child
    pid_t child_pid = fork();
    if (child_pid == -1) {
        std::cerr << "PID " << pid << ": Failed to fork" << std::endl;
        close(fd);
        return 1;
    }

    if (child_pid == 0) { // Child
        pid = getpid(); // Update PID for child
        // Close descriptor to ensure lock release
        close(fd);
        std::cout << "PID " << pid << ": Child: File descriptor closed" << std::endl;

        // Child runs indefinitely
        while (true) {
            std::cout << "PID " << pid << ": Child: Running indefinitely..." << std::endl;
            sleep(5); // Print every 5 seconds
        }
        exit(0); // Unreachable
    } else { // Parent
        // Write to file
        std::string data = "Parent writing to file\n";
        write(fd, data.c_str(), data.size());
        std::cout << "PID " << pid << ": Parent: Wrote to file" << std::endl;

        // Parent exits
        std::cout << "PID " << pid << ": Parent: Exiting" << std::endl;
        flock(fd, LOCK_UN);
        close(fd);
        return 0;
    }

    return 0;
}
