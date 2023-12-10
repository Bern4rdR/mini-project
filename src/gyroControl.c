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
#define SAMPLE_TIME 250



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
    float potentiometerFloat = (float)readADC() / 1023;
    // convert to a float between 0 and 2pi
    *playerDirection = potentiometerFloat * 2 * PI;
}

void initADC() {
    /* PORTB.2 is analog pin with potentiometer*/
	AD1PCFG = ~(1 << 2);
	TRISBSET = (1 << 2);
	/* Use pin 2 for positive */
	AD1CHS = (0x2 << 16);
	
	/* Data format in uint32, 0 - 1024
	Manual sampling, auto conversion when sampling is done
	FORM = 0x4; SSRC = 0x7; CLRASAM = 0x0; ASAM = 0x0; */
	AD1CON1 = (0x4 << 8) | (0x7 << 5);
	
	AD1CON2 = 0x0;
	AD1CON3 |= (0x1 << 15);
	
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1 |= (0x1 << 15);    
}

// void initTimer() {
//     // initialize timer 1
//     T1CON = 0x70; // set prescaler to 256
//     PR1 = 3125; // set period to 0.1 seconds
//     TMR1 = 0; // reset timer
//     T1CONSET = 0x8000; // start timer

//     // set timer interrupt
//     IPCSET(2) = 0x1C; // set priority to 7
//     IFSCLR(0) = 0x100; // clear interrupt flag
//     IECSET(0) = 0x100; // enable interrupt


//     // the timer ticks every 0.1 seconds

//     // wait until timer interrupt flag is set
//     while (!(IFS(0) & 0x100));
//     // reset timer interrupt flag
//     IFSCLR(0) = 0x100;

//     // reset timer
//     TMR1 = 0;

// }


int readADC() {
    /* Start sampling, wait until conversion is done */
    AD1CON1 |= (0x1 << 1);
    while(!(AD1CON1 & (0x1 << 1)));
    while(!(AD1CON1 & 0x1));

    return ADC1BUF0; // read the buffer with the result
}

int getbtns(void) {
    // returns the value of the buttons
    return (PORTD >> 5) & 0x7;
}