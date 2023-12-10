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

    set_interrupts();

    open_ports();

    initTimer();
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
            // can remove for actual game
            display_string(textbuffer, 1, "BTN4 pressed");
            display_update(textbuffer);
            
            // keep for actual game
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
    
    int mapping[128] = {
        1, 1, 1, 1, 1, 1, 1, 1, // 0-7,
        1, 0, 0, 0, 0, 0, 0, 0, // 8-15,
        1, 0, 0, 0, 0, 0, 0, 0, // 16-23
        1, 0, 0, 0, 0, 0, 0, 0, // 24-31
        1, 0, 0, 0, 0, 1, 0, 0, // 32-39
        1, 0, 0, 0, 1, 1, 0, 0, // 40-47
        1, 0, 0, 0, 0, 0, 0, 0, // 48-55
        1, 1, 1, 1, 1, 1, 1, 1, // 56-63
    // 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63
    };
    
    int playerPosX = 15, playerPosY = 31;
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
    

    PORTD = (~0x4 << 5);

    // // game loop
    while (1) {
        castRay(&playerDirection, &playerPosX, &playerPosY, mapping, 8, display);

        user_isr(&walking, &playerDirection);

        if (getbnts() & 0x4) {
            //movePlayer(&playerDirection, &playerPosX, &playerPosY, mapping, 8);
            playerPosX = 40;
            playerPosY = 50;
        }
        render_display(display);
    }
}