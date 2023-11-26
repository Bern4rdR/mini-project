/*
 * This file contains functions to calculate the screen.
 * Using information from the game logic it will know where 
 * and how to draw the walls
 */

#define DISPLAY_HEIGHT 720
#define DISPLAY_WIDTH  1280
#define BYTE_SIZE      8


/* Draw a vertical line with specified height and opacity
 * @param line:     pointer to the line to be drawn
 * @param distance: distance from the top of the screen
 * @param opacity:  opacity of the line (0-1)
 */
void drawLine(unsigned int* line, int distance, float opacity) {
    // Clear line
    for (int i = 0; i < DISPLAY_HEIGHT/BYTE_SIZE; i++) {
        line[i] = 0;
    }

    // Calculate the height of the line
    int height = DISPLAY_HEIGHT - distance;
    int dither = height * opacity;

    // Set the pixels on the line
    int i;
    for (i = distance/2; i < DISPLAY_HEIGHT - distance/2; i++) {
        // Use opacity to simulate color
        line[i/BYTE_SIZE] |= (i % dither) ? 1UL << i % BYTE_SIZE : 0UL << i % BYTE_SIZE;
    }
}

void panScreen(unsigned int* display[], int distance) {
    // Shift all bits in display by distance
    // If distance is positive, shift left
    // If distance is negative, shift right
    // If distance is 0, do nothing

    // Shift left with matrix multiplication (each array of ints is a column)
    
    // Shift right with matrix multiplication (each array of ints is a column)
}


int main() {
    // Display with 1x720 resolution:  initialize all bits to 0
    unsigned char display[DISPLAY_HEIGHT/BYTE_SIZE] = {0};

    // Test:  distance can't be more than DISPLAY_HEIGHT
    // In the future, display will be a 2D array and only one column will be passed through
    drawLine(&display, 360, 50);

    return 0;
}
