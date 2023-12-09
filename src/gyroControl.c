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
#define POTENTIOMETER A0


/* Handle the inputs from the user, by reading the potentiometer and
 * the buttons and setting the status accordingly
 * @param walking: pointer to a boolean that indicates if the player is walking or not
 * @param playerDirection: pointer to a float that indicates the direction the player is facing
 */
void user_isr(int* walking, float* playerDirection) {
    if (getbtns() & BTN4) {
        *walking = *walking ? 0 : 1;
    }

    // convert to a float between 0 and 1
    float potentiometerFloat = (float)ReadADC10(POTENTIOMETER) / 1023;
    // convert to a float between 0 and 2pi
    *playerDirection = potentiometerFloat * 2 * PI;
}

int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}