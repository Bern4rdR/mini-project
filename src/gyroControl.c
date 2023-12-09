/*
 * This file will read and handle the potentiometer data and translate it into directions
 * that can be used to control the player. Turning the potentiometer will make the player
 * turn in that direction. Pressing button1 will make the player walk forward.
 */
// #include <plib.h> // PIC32 peripheral library
#include <pic32mx.h>
#include <p32xxxx.h>
#include "control.h"

// math constants
#define PI 3.1415926535

// pin constants
#define BTN4 0x4
#define POTENTIOMETER 14
#define SAMPLE_TIME 250



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
    AD1CON1 = 0x00E0; // automatic conversion after sampling
    AD1CHS = 0x0000; // connect AN0 as S/H input
    AD1CSSL = 0;
    AD1CON3 = 0x1F02; // Tad = 2 Tcy
    AD1CON2 = 0;
}

int readADC() {
    AD1CON1bits.SAMP = 1; // start sampling, then go to conversion

    while (!AD1CON1bits.DONE); // wait until conversion is done

    return ADC1BUF0; // read the buffer with the result
}

int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}