#include <avr/io.h>
#include <util/delay.h>

// Define states
#define STATE_IDLE 1
#define STATE_RIGHT 2
#define STATE_LEFT 3
#define STATE_HAZARD 4
#define STATE_STOP 5

// Variables
uint8_t state = STATE_IDLE;

// Button debouncing function
uint8_t is_button_pressed() {
    if (!(PIND & (1 << PD2))) {
        _delay_ms(50); // Debouncing delay
        if (!(PIND & (1 << PD2))) {
            return 1;
        }
    }
    return 0;
}

// Function to display pattern with multiplexing
void display_pattern(uint8_t pattern[6]) {
    for (uint8_t row = 0; row < 6; row++) {
        // Set row output (PC0-PC5 as row)
        DDRC = 0b00111111; // Set PC0-PC5 as output
        PORTC = ~(1 << row);  // Select the row (active low)

        // Set column output for that row
        DDRC = 0b00111111; // Ensure PC0-PC5 are outputs for columns
        PORTC = pattern[row]; // Set column pattern

        _delay_ms(2);  // Short delay for multiplexing
    }
}

// Function to switch to next state on button press
void change_state() {
    if (is_button_pressed()) {
        state++;
        if (state > STATE_STOP) {
            state = STATE_IDLE;
        }
    }
}

int main(void) {
    // Setup PC0-PC5 as output for matrix columns and rows (multiplexed)
    DDRC = 0b00111111;
    // Setup PD2 as input for button
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2); // Enable pull-up resistor for button

    // Define patterns
    uint8_t idle_pattern[6] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};

    // Corrected right arrow pattern (pointing right)
    uint8_t right_arrow[6] = {
        0b00001000, //   ▢▢▢▣▢▢▢
        0b00001100, //   ▢▢▢▣▣▢▢
        0b00111111, //   ▢▢▣▣▣▣▣
        0b00111111, //   ▢▢▣▣▣▣▣
        0b00001100, //   ▢▢▢▣▣▢▢
        0b00001000  //   ▢▢▢▣▢▢▢
    };

    // Corrected left arrow pattern (pointing left)
    uint8_t left_arrow[6] = {
        0b00010000, //   ▢▣▢▢▢▢▢
        0b00110000, //   ▣▣▢▢▢▢▢
        0b11111100, //   ▣▣▣▣▣▣▢
        0b11111100, //   ▣▣▣▣▣▣▢
        0b00110000, //   ▣▣▢▢▢▢▢
        0b00010000  //   ▢▣▢▢▢▢▢
    };

    // Corrected stop pattern (central point expanding outward)
    uint8_t stop_pattern[6] = {
        0b00011000, //   ▢▢▣▣▢▢
        0b00100100, //   ▢▣▢▢▣▢
        0b01000010, //   ▣▢▢▢▢▣
        0b01000010, //   ▣▢▢▢▢▣
        0b00100100, //   ▢▣▢▢▣▢
        0b00011000  //   ▢▢▣▣▢▢
    };

    uint8_t hazard_pattern[6] = {0b00111110, 0b00100010, 0b00010100, 0b00001000, 0b00010100, 0b00100010}; // Blinking triangle

    // Main loop
    while (1) {
        change_state(); // Check button and change state

        switch (state) {
            case STATE_IDLE:
                display_pattern(idle_pattern);
                break;

            case STATE_RIGHT:
                display_pattern(right_arrow);
                break;

            case STATE_LEFT:
                display_pattern(left_arrow);
                break;

            case STATE_HAZARD:
                display_pattern(hazard_pattern);
                _delay_ms(300); // Blinking delay
                display_pattern(idle_pattern);
                _delay_ms(300); // Blinking delay
                break;

            case STATE_STOP:
                display_pattern(stop_pattern);
                break;
        }
    }
    return 0;
}
