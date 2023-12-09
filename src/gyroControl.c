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


/* Handle interrupts from button1 and the potentiometer,and change states of the game
 * @param walking: pointer to a boolean that indicates if the player is walking or not
 * @param playerDirection: pointer to a float that indicates the direction the player is facing
 */
void user_isr(int* walking, float* playerDirection) {
    // acknowledge interrupt from button1
    if (IFS(0) & 0x100) {
        IFSCLR(0) = 0x100;
        *walking = *walking ? 0 : 1;
    }

    // acknowledge interrupt from the potentiometer
    if (IFS(0) & 0x800) {
        IFSCLR(0) = 0x800;
        // read the potentiometer value
        int potentiometer = (PORTA & 0x0f00) >> 8;
        // convert to a float between 0 and 1
        float potentiometerFloat = (float)potentiometer / 15;
        // convert to a float between 0 and 2pi
        *playerDirection = potentiometerFloat * 2 * PI;
    }
}

void set_interrupts(void) {
    // enable interrupts from button1
    IECSET(0) = 0x100;
    // enable interrupts from the potentiometer
    IECSET(0) = 0x800;

    // set priority of interrupts from button1
    IPCSET(1) = 0x1c;
    // set priority of interrupts from the potentiometer
    IPCSET(2) = 0x1c;
}

void open_ports(void) {
    // open the port for button1
    TRISFSET = 0x2;
    // open the port for the potentiometer
    TRISASET = 0x0f00;
}


int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}