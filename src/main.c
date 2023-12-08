#include <pic32mx.h>
#include <stdint.h>
#include <string.h>


#include "render.h"
#include "control.h"
#include "screen.h"
#include "math.h"


#define DISPLAY_WIDTH 128


void init() {
    /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

    
    // initialize the display
    display_init();
    // initialize the buttons and potentiometer
    open_ports();
    // initialize interrupts
    set_interrupts();

    clear_display();
}


void main() {
    char textbuffer[4][16];
    init();

    // create textbuffer for displaying text
    create_textbuffer(textbuffer);

    display_string(textbuffer, 0, "Hello world!");

    display_update(textbuffer);

    char display[4][DISPLAY_WIDTH];
    // clear display
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < DISPLAY_WIDTH; j++) {
            display[i][j] = 0;
        }
    }
    int map[] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 1, 0, 0, 1,
        1, 0, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };
    int playerPosX = 54, playerPosY = 54;
    float playerDirection = 0;

    castRay(&playerDirection, &playerPosX, &playerPosY, map, 8, display);

    display_update(display);
}