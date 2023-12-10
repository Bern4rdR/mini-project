#include <pic32mx.h>
#include <stdint.h>
//#include <string.h>


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
    erase_display();

    // initialize the ADC module (to read potentiometer)
    initADC();

    // initialize timer 1
    T1CON = 0x70; // set prescaler to 256
    PR1 = 3125; // set period to 0.1 seconds
    TMR1 = 0; // reset timer
    T1CONSET = 0x8000; // start timer

    // set timer interrupt
    IPCSET(2) = 0x1C; // set priority to 7
    IFSCLR(0) = 0x100; // clear interrupt flag
    IECSET(0) = 0x100; // enable interrupt
}

void tick() {
    // wait until timer interrupt flag is set
    while (!(IFS(0) & 0x100));
    // reset timer interrupt flag
    IFSCLR(0) = 0x100;

    // reset timer
    TMR1 = 0;
}

void main() {
    char textbuffer[4][16];

    init();

    // create textbuffer for displaying text
    create_textbuffer(textbuffer);

    clear_display(textbuffer);

    display_update(textbuffer);

    quicksleep(5000);

    // MENU 

    display_string(textbuffer, 0, "Maze Runner");
    
    display_string(textbuffer, 2, "Press BTN4");
    display_string(textbuffer, 3, "to start");

    display_update(textbuffer);

    while (1) {
        // if button 4 is pressed - start game
        if (getbtns() & 0x4) {
            display_string(textbuffer, 1, "BTN4 pressed");
            display_update(textbuffer);
            quicksleep(500000);
            clear_display(textbuffer);
            display_update(textbuffer);
            break;
        }
    }
   
    // GAME

    char display[4][DISPLAY_WIDTH];
    // clear display
    
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < DISPLAY_WIDTH; j++) {
            display[i][j] = 0;
        }
    }
    
    int mapping[64] = {
        1, 1, 1, 1, 1, 1, 1, 1, // 0-8, 8-16, 16-24, 24-32, 32-40, 40-48, 48-56, 56-64
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };
    
    int playerPosX = 32, playerPosY = 32;
    float playerDirection = 0;
    int walking = 0;

    float pot_value = readADC() / 1023;
    

    // while (1) {
    //     // potentiometer test:
    //     display_string(textbuffer, 0, "Potentiometer");
    //     display_update(textbuffer);

    //     float new_pot_value = readADC() / 1023;
    //     if (new_pot_value != pot_value) {
    //         display_string(textbuffer, 1, "rotated");
    //         display_update(textbuffer);
    //     }
    // }
    

    

    // // game loop
    while (1) {
        castRay(&playerDirection, &playerPosX, &playerPosY, mapping, 8, display);

        user_isr(&walking, &playerDirection);

        if (walking) {
            movePlayer(&playerDirection, &playerPosX, &playerPosY, mapping, 8);
        }
        render_display(display);
        tick();
    }
}