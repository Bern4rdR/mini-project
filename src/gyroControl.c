/*
 * This file will read and handle the potentiometer data and translate it into directions
 * that can be used to control the player. Turning the potentiometer will make the player
 * turn in that direction. Pressing button1 will make the player walk forward.
 */
// #include <plib.h> // PIC32 peripheral library
#include <pic32mx.h>
#include "control.h"


// math constants
#define PI 3.1415926535

// pin constants
#define BTN4 0x4
#define POTENTIOMETER 0



/* Handle the inputs from the user, by reading the potentiometer and
 * the buttons and setting the status accordingly
 * @param walking: pointer to a boolean that indicates if the player is walking or not
 * @param playerDirection: pointer to a float that indicates the direction the player is facing
 */
void user_input(int* walking, float* playerDirection) {
    if (getbtns() & BTN4) {
        *walking = *walking ? 0 : 1;
    }

    // convert to a float between 0 and 1
    float potentiometerFloat = (float)analogRead(A0) / 1023;
    // convert to a float between 0 and 2pi
    *playerDirection = potentiometerFloat * 2 * PI;
}

// Initialize the ADC module to read the potentiometer value
// void initADC() {
//     AD1PCFG = 0xFFFB; // Set PORTB<2> as analog input
//     AD1CON1 = 0x00E0; // Set auto-convert mode
//     AD1CSSL = 0; // No scanning required
//     AD1CON2 = 0; // Use MUXA, AVss and AVdd as Vref
//     AD1CON3 = 0x1F3F; // Set Tad and sample time
//     AD1CON1SET = 0x8000; // Enable the ADC module
// }

// Read the ADC value from channel 2 (AN2)
// int readADC() {
//   AD1CHSbits.CH0SA = 2; // Select AN2 as input
//   AD1CON1bits.SAMP = 1; // Start sampling
//   while (!AD1CON1bits.DONE); // Wait for conversion to finish
//   return ADC1BUF0; // Return the ADC value
// }

int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}