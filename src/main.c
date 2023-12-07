#include <pic32mx.h>


#include "render.h"
#include "control.h"
#include "screen.h"


char textbuffer[4][16];


void init() {
    // initialize the display
    display_init();
    // initialize the buttons and potentiometer
    open_ports();
    // initialize interrupts
    set_interrupts();

    // create textbuffer for displaying text
    create_textbuffer(textbuffer);
}


void main() {
    init();

    display_string(textbuffer, 0, "Hello world!");

    //menu();
}