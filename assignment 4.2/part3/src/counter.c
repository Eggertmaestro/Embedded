#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define DEVICE_PATH "/dev/mydev1"

int main() {
    int fd;
    char buffer[256];
    ssize_t bytesWritten;

    // Open the device
    fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // Loop to write to the device every second
    while (1) {
        // Generate and write dummy data to the device
        int numBytes = rand() % 256 + 1;
        snprintf(buffer, sizeof(buffer), "Hello %d\n", numBytes);
        bytesWritten = write(fd, buffer, numBytes);
        if (bytesWritten < 0) {
            perror("Failed to write to the device");
            break;
        }
        sleep(1);
    }

    // Close the device
    if (close(fd) < 0) {
        perror("Failed to close the device");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}