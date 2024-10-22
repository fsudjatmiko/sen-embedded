# LED Matrix Turn Signal Project

This project implements a 6x6 LED matrix display that simulates various turn signal patterns for a vehicle. It's designed to run on an AVR microcontroller (e.g., ATmega328P) and uses multiplexing to control the LED matrix.

## Features

- 5 display states:
  1. Idle (blank display)
  2. Right turn signal
  3. Left turn signal
  4. Hazard warning (blinking triangle)
  5. Stop signal
- Button-controlled state switching
- Debounced button input
- Efficient LED matrix multiplexing

## Hardware Requirements

- AVR microcontroller (e.g., ATmega328P)
- 6x6 LED matrix
- Push button
- Appropriate resistors for LEDs and button
- AVR programmer

## Pin Connections

- PC0-PC5: Connected to LED matrix rows and columns (multiplexed)
- PD2: Connected to push button (with pull-up resistor enabled)

## How It Works

1. The program initializes the microcontroller's ports and sets up the button input.
2. It enters a main loop where it continuously checks for button presses and updates the display.
3. When the button is pressed, the state changes to the next in sequence.
4. The LED matrix display is updated using multiplexing technique, cycling through rows rapidly to create a persistent image.

## States and Patterns

1. Idle: Blank display
2. Right Turn: Right-pointing arrow
3. Left Turn: Left-pointing arrow
4. Hazard: Blinking triangle
5. Stop: Expanding point pattern

## Compilation and Upload

This project is designed to be compiled and uploaded using a compatible AVR toolchain. Ensure you have the necessary tools installed (e.g., avr-gcc, avrdude) and your programmer is correctly configured.

## Future Improvements

- Add brightness control
- Implement smooth transitions between states
- Add more complex patterns or animations