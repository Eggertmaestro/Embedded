#include <gpiod.h>  // The libgpiod header
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()

#define GPIO_INPUT_PIN 12       // Input GPIO pin number
#define GPIO_OUTPUT_PIN 17      // Output GPIO pin number
#define GPIO_CHIP "/dev/gpiochip0" // The GPIO chip (usually gpiochip0 on Raspberry Pi)

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *input_line, *output_line;
    int input_value;

    // Open the GPIO chip
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Open GPIO chip failed");
        return EXIT_FAILURE;
    }

    // Get the input GPIO line (pin)
    input_line = gpiod_chip_get_line(chip, GPIO_INPUT_PIN);
    if (!input_line) {
        perror("Get input GPIO line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Get the output GPIO line (pin)
    output_line = gpiod_chip_get_line(chip, GPIO_OUTPUT_PIN);
    if (!output_line) {
        perror("Get output GPIO line failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request the input line
    if (gpiod_line_request_input(input_line, "input_polling") < 0) {
        perror("Request input line as input failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Request the output line
    if (gpiod_line_request_output(output_line, "output_copy", 0) < 0) {
        perror("Request output line as output failed");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // Poll the input pin status in a loop
    while (1) {
        // Read the input pin value (0 or 1)
        input_value = gpiod_line_get_value(input_line);
        if (input_value < 0) {
            perror("Read input line value failed");
            break;  // Exit the loop on error
        }
        
        // Copy the input value to the output pin
        gpiod_line_set_value(output_line, input_value);

        // Sleep for 100ms before polling again (adjust the interval as needed)
        usleep(1);  // 100ms = 100,000 microseconds
    }

    // Release the GPIO lines and close the chip
    gpiod_line_release(input_line);
    gpiod_line_release(output_line);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}