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
#define POTENTIOMETER 14



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
    float potentiometerFloat = (float)readADC() / 1023;
    // convert to a float between 0 and 2pi
    *playerDirection = potentiometerFloat * 2 * PI;
}
void initADC() {
    // Configure the ADC module
    AD1PCFG = 0xFFFB; // All PORTB = Digital; RB2 = analog
    AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
    // and starts converting
    AD1CHS = POTENTIOMETER << 16; // Connect RB2/AN2 as CH0 input ..
    // in this example RB2/AN2 is the input
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
    AD1CON2 = 0;
    AD1CON1SET = 0x8000; // turn ADC ON
}

int readADC() {
    int elapsed = 0, finish_time = 0;
    AD1CON1bits.SAMP = 1;            // start sampling ...
    elapsed = _CP0_GET_COUNT(); // get current timer count
    finish_time = elapsed + SAMPLE_TIME; // Set finish time
    while (_CP0_GET_COUNT() < finish_time); // sample for more than 250 ns
    AD1CON1bits.SAMP = 0; // stop sampling and start converting
    while (!AD1CON1bits.DONE); // wait for the conversion process to finish
    return ADC1BUF0; // read the buffer with the result
}

int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}