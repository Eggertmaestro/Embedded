#include <stdio.h>
#include <stdlib.h>
#include <gpiod.h>
#include <unistd.h>
#include <signal.h>

#define CONSUMER "GPIO_Event_Watcher" // Consumer name
#define GPIO_INPUT_LINE_NUMBER 12       // Replace with your actual GPIO input line number
#define GPIO_OUTPUT_LINE_NUMBER 17      // Replace with your actual GPIO output line number

static volatile int running = 1;

// Signal handler for cleanup on Ctrl+C
void handle_signal(int signal) {
    running = 0;
}

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *input_line;
    struct gpiod_line *output_line;
    struct gpiod_line_event event;
    int ret;

    // Set up signal handler for graceful exit
    signal(SIGINT, handle_signal);

    // Open the GPIO chip
    chip = gpiod_chip_open_by_number(0); // 0 is usually the first GPIO chip, change if necessary
    if (!chip) {
        perror("Open GPIO chip failed");
        return EXIT_FAILURE;
    }

    // Get the desired GPIO input line
    input_line = gpiod_chip_get_line(chip, GPIO_INPUT_LINE_NUMBER);
    if (!input_line) {
        perror("Get input line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Get the desired GPIO output line
    output_line = gpiod_chip_get_line(chip, GPIO_OUTPUT_LINE_NUMBER);
    if (!output_line) {
        perror("Get output line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request the input line as input
    if (gpiod_line_request_both_edges_events(input_line, CONSUMER) < 0) {
        perror("Request events for input line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request the output line
    if (gpiod_line_request_output(output_line, CONSUMER, 0) < 0) {
        perror("Request output line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Event listening loop
    while (running) {
        // Wait for an event
        ret = gpiod_line_event_wait(input_line, NULL);
        if (ret < 0) {
            perror("Wait for event failed");
            break;
        }
        if (ret == 0) {
            continue;  // Timeout
        }

        // Read the event
        ret = gpiod_line_event_read(input_line, &event);
        if (ret < 0) {
            perror("Read event failed");
            break;
        }

        // Handle the detected edge
        if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            gpiod_line_set_value(output_line, 1); // Set output high
        } else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            gpiod_line_set_value(output_line, 0); // Set output low
        }
    }

    // Cleanup
    gpiod_line_release(input_line);
    gpiod_line_release(output_line);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}