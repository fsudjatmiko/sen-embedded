#include <avr/io.h>
#include <util/delay.h>

#define LED_MATRIX_PORT PORTC
#define LED_MATRIX_DDR DDRC

#define TURN_RIGHT PD2
#define TURN_LEFT PD3
#define HAZARD_BUTTON PD4

#define DEBOUNCE_DELAY 50 // 50ms debounce delay

// Store the state of the LEDs
uint8_t ledMatrix[6][6] = {0}; // For 6x6 matrix

void setup() {
    // Set PC0 to PC5 as output for LED matrix columns
    LED_MATRIX_DDR |= 0b00111111; // PC0-PC5 as output
    
    // Set PD0 to PD5 as output for rows (assuming PD0-PD5 control rows 2-7)
    DDRD |= 0b00111111;

    // Set PD2, PD3, PD4 as input for switches and the button
    DDRD &= ~(1 << TURN_RIGHT) & ~(1 << TURN_LEFT) & ~(1 << HAZARD_BUTTON);
    
    // Enable pull-up resistors for inputs
    PORTD |= (1 << TURN_RIGHT) | (1 << TURN_LEFT) | (1 << HAZARD_BUTTON);
}

uint8_t isButtonPressed(uint8_t pin) {
    if (!(PIND & (1 << pin))) { // Check if the button is pressed (active low)
        _delay_ms(DEBOUNCE_DELAY); // Wait for debounce delay
        if (!(PIND & (1 << pin))) { // Check again if the button is still pressed
            return 1; // Button press confirmed
        }
    }
    return 0; // Button not pressed
}

void setMatrix(uint8_t colMask, uint8_t row) {
    // Set the columns and activate the row
    LED_MATRIX_PORT = colMask; // Set columns
    PORTD &= ~(1 << (row - 2)); // Activate the row (PD0-PD5 control rows 2-7)
    _delay_ms(1); // Short delay to allow the LEDs to light up
    PORTD |= (1 << (row - 2)); // Deactivate the row
}

void multiplex() {
    for (uint8_t row = 2; row <= 7; row++) {
        // Create the column mask for the current row
        uint8_t colMask = 0;

        // Create a mask based on the ledMatrix state
        for (uint8_t col = 2; col <= 7; col++) {
            if (ledMatrix[row - 2][col - 2]) { // Check if the LED should be on
                colMask |= (1 << (col - 2)); // Set the corresponding column bit
            }
        }

        // Update the matrix for this row
        setMatrix(colMask, row);
    }
}

void turnRightSignal() {
    // Set up the diagonal for right turn signal
    for (uint8_t i = 0; i < 6; i++) {
        // Light up the diagonal for right turn signal
        ledMatrix[i][i] = 1; // Set the corresponding LED to on
    }

    // Multiplex to display the matrix
    for (uint8_t i = 0; i < 10; i++) { // Repeat to make it visible
        multiplex();
        _delay_ms(100); // Adjust speed as needed
    }

    // Clear the matrix
    for (uint8_t i = 0; i < 6; i++) {
        ledMatrix[i][i] = 0; // Clear the diagonal
    }
}

void turnLeftSignal() {
    // Set up the diagonal for left turn signal
    for (uint8_t i = 0; i < 6; i++) {
        ledMatrix[i][5 - i] = 1; // Set the corresponding LED to on
    }

    // Multiplex to display the matrix
    for (uint8_t i = 0; i < 10; i++) { // Repeat to make it visible
        multiplex();
        _delay_ms(100); // Adjust speed as needed
    }

    // Clear the matrix
    for (uint8_t i = 0; i < 6; i++) {
        ledMatrix[i][5 - i] = 0; // Clear the diagonal
    }
}

void hazardSignal() {
    while (isButtonPressed(HAZARD_BUTTON)) { // Keep the hazard lights on while the button is pressed
        // Set all LEDs on for hazard
        for (uint8_t row = 0; row < 6; row++) {
            for (uint8_t col = 0; col < 6; col++) {
                ledMatrix[row][col] = 1; // Set all to on
            }
        }
        
        multiplex(); // Update the matrix to show all LEDs on
        _delay_ms(250); // Delay for half of the blinking time
        for (uint8_t row = 0; row < 6; row++) {
            for (uint8_t col = 0; col < 6; col++) {
                ledMatrix[row][col] = 0; // Set all to off
            }
        }
        multiplex(); // Update the matrix to show all LEDs off
        _delay_ms(250); // Delay for the other half of the blinking time
    }
}

int main() {
    setup();
    
    while (1) {
        if (!(PIND & (1 << TURN_RIGHT))) {
            turnRightSignal(); // Activate right turn signal
        } 
        else if (!(PIND & (1 << TURN_LEFT))) {
            turnLeftSignal(); // Activate left turn signal
        } 
        else if (isButtonPressed(HAZARD_BUTTON)) {
            hazardSignal(); // Activate hazard lights
        } 
        else {
            // Clear the LED matrix if no signals are active
            for (uint8_t row = 0; row < 6; row++) {
                for (uint8_t col = 0; col < 6; col++) {
                    ledMatrix[row][col] = 0; // Set all to off
                }
            }
            multiplex(); // Update the matrix to show all LEDs off
        }
    }
}
