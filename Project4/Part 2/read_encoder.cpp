#include "pi_control.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <thread>

double read_value_from_file(const char* file_path) {
    char buffer[16];
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device file");
        return 0.0;
    }
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        return atof(buffer);  // Convert string to double
    }
    return 0.0;
}

void set_pwm_duty_cycle(double duty_cycle) {
    FILE *pwm_fd = fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", "w");
    if (pwm_fd) {
        fprintf(pwm_fd, "%d", (int)duty_cycle); // Duty cycle assumption (integer value)
        fclose(pwm_fd);
        printf("PWM duty cycle set to: %.2f\n", duty_cycle);
    } else {
    }
}

int main(int argc, char *argv[]) {
    double reference_speed = atof(argv[1]);
    PI_Control pi_controller(1.0, 0.5, 0.1, 1000);
    std::vector<long long> execution_times;
    auto target_interval = std::chrono::milliseconds(100);
    constexpr size_t BATCH_SIZE = 100;

    while (true) {
        auto start_time = std::chrono::steady_clock::now();

        double actual_speed = read_value_from_file("/dev/encoder_speed");
        auto current_time = std::chrono::steady_clock::now();
        double dt = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() / 1000.0;

        double pwm_output = pi_controller.update(reference_speed, actual_speed, dt);
        set_pwm_duty_cycle(pwm_output);

        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        execution_times.push_back(duration);

        // writing the execution times in batches to reduce jitter
        if (execution_times.size() >= BATCH_SIZE) {
            std::ofstream outfile("execution_times.txt", std::ios::app);
            for (const auto& time : execution_times) {
                outfile << time << "\n";
            }
            execution_times.clear();
        }

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        if (elapsed < target_interval) {
            std::this_thread::sleep_for(target_interval - elapsed);
        }
    }

    return 0;
}
