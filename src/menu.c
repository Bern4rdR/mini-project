/* This file fixes the main/start menu for when the code/game is initialized */

#include <pic32mx.h>
#include <stdint.h>


/* Display a main menu state on the ChipKit display
 */
void menu(){
    // clears the display to get an empty canvas
    clear_display();

    // display the menu
    display_string(0, "Welcome to the");
    display_string(1, "ChipKIT Maze");
    display_string(2, "Press button 1");
    display_string(3, "to start");
    display_update();
    
    // wait for button 1 to be pressed
    while(!(getbtns() & 0x1));
    
    // clear the display
    clear_display();
}

void start_game(){
    // 
}