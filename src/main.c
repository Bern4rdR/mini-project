#include <pic32mx.h>
#include <stdint.h>

#include "render.h"
#include "control.h"
#include "screen.h"
#include "math.h"


#define DISPLAY_WIDTH 128
// button constants
#define BTN4 0x4
#define BTN3 0x2
#define BTN2 0x1
// switch constants
#define SW4 0x8

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

void menu_loop(int* map_select, char textbuffer[4][16]) {
    // MENU 
    display_string(textbuffer, 1, "Maze Runner");

    display_update(textbuffer);

    quicksleep(5000000);

    // menu loop
    while (1) {
        display_string(textbuffer, 0, "Select map SW4");
        display_string(textbuffer, 1, "BTN4 to start");

        if(getsw() & SW4) {
            display_string(textbuffer, 2, " Map 1");
            display_string(textbuffer, 3, "[Map 2]");
            *map_select = 1;
        }
        else {
            display_string(textbuffer, 2, "[Map 1]");
            display_string(textbuffer, 3, " Map 2");
            *map_select = 0;
        }

        display_update(textbuffer);
        
        // if button 4 is pressed - start game
        if (getbtns() & 0x4) {
            // keep for actual game
            quicksleep(500000);
            clear_display(textbuffer);
            display_update(textbuffer);
            return;
        }
    }
}

void game_loop(int map[64], char display[4][DISPLAY_WIDTH]) {
    int playerPosX = 10, playerPosY = 27;
    float playerDirection = 0;
    int walking = 0;
    int mapSize = 8;

    float pot_value = readADC() / 1023;
    

    // game loop
    while (1) {
        castRay(&playerDirection, &playerPosX, &playerPosY, map, mapSize, display);

        user_isr(&walking, &playerDirection);

        if (getbtns() & 0x2) {
            //movePlayer(&playerDirection, &playerPosX, &playerPosY, map, mapSize);
            // determine direction of player
            int moveX = cos(playerDirection) + 0.5; // roundabout way of rounding to the nearest integer
            int moveY = sin(playerDirection) + 0.5;

            // use moveX and moveY to determine the direction of the player (8 possibilities)
            // down is positive, right is positive
            if (moveX > 0 && moveY > 0) {
                // player is moving up and right
                playerPosX += 1;
                playerPosY -= 1;
            } else if (moveX > 0 && moveY < 0) {
                // player is moving down and right
                playerPosX += 1;
                playerPosY += 1;
            } else if (moveX < 0 && moveY > 0) {
                // player is moving up and left
                playerPosX -= 1;
                playerPosY -= 1;
            } else if (moveX < 0 && moveY < 0) {
                // player is moving down and left
                playerPosX -= 1;
                playerPosY += 1;
            } else if (moveX > 0 && moveY == 0) {
                // player is moving right
                playerPosX += 1;
            } else if (moveX < 0 && moveY == 0) {
                // player is moving left
                playerPosX -= 1;
            } else if (moveX == 0 && moveY > 0) {
                // player is moving up
                playerPosY -= 1;
            } else if (moveX == 0 && moveY < 0) {
                // player is moving down
                playerPosY += 1;
            }
        }

        render_display(display);
    }

    // exit if button 1 is pressed
    if (getbtns() & 0x1) {
        return;
    }
}

void main() {
    char textbuffer[4][16];

    init();

    // create textbuffer for displaying text
    create_textbuffer(textbuffer);

    clear_display(textbuffer);

    display_update(textbuffer);

    quicksleep(5000);
   
    // GAME
    char display[4][DISPLAY_WIDTH];
    
    // clear display
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < DISPLAY_WIDTH; j++) {
            display[i][j] = 0;
        }
    }
    
    int map1[64] = {
        1, 1, 1, 1, 1, 1, 1, 1, // 0-7,
        1, 0, 0, 1, 0, 0, 0, 1, // 8-15,
        1, 0, 1, 1, 0, 1, 1, 1, // 16-23
        1, 0, 0, 0, 0, 0, 0, 1, // 24-31
        1, 0, 1, 0, 0, 1, 0, 1, // 32-39
        1, 0, 0, 0, 1, 1, 0, 1, // 40-47
        1, 0, 0, 0, 0, 0, 0, 1, // 48-55
        1, 1, 1, 1, 1, 1, 1, 1  // 56-63
    // 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63
    };

    int map2[64] = {
        1, 1, 1, 1, 1, 1, 1, 1, // 0-7,
        1, 0, 0, 1, 0, 0, 0, 1, // 8-15,
        1, 0, 1, 1, 1, 0, 1, 1, // 16-23
        1, 0, 0, 0, 0, 0, 0, 1, // 24-31
        1, 0, 1, 0, 0, 1, 0, 1, // 32-39
        1, 0, 1, 1, 1, 1, 0, 1, // 40-47
        1, 0, 0, 1, 0, 0, 0, 1, // 48-55
        1, 1, 1, 1, 1, 1, 1, 1  // 56-63
    // 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63
    };

    int* maps[2] = {map1, map2};
    
    while(1) {
        int map_select = 0;
        menu_loop(&map_select, textbuffer);

        game_loop(maps[map_select], display);
    }
}