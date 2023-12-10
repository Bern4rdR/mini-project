/*
 * This file contains functions to calculate the screen.
 * Using information from the game logic it will know where 
 * and how to draw the walls
 */

// display constants
#define DISPLAY_HEIGHT 32       // screen pixels 
#define DISPLAY_WIDTH  128      // screen pixels
#define BYTE_SIZE      8

// math constants
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.01745329251


#include <stdio.h>
//#include <string.h>
#include "screen.h"
#include "math.h"


/* Draw a vertical line with specified height and opacity
 * @param display:  the display on which the line will be drawn
 * @param col:      the column on which the line will be drawn
 * @param distance: distance to the wall
 * @param opacity:  opacity of the line (0-1)
 */
void drawLine(char display[4][DISPLAY_WIDTH], int col, float distance, float opacity) {
    int i;
    // clear column
    for (i = 0; i < 4; i++) {
        display[i][col] = 0;
    }

    // calculate the height of the line
    // map distance (can be 0-48) to 0-1
    float height = (DISPLAY_HEIGHT * 0.5) / distance;
    // map height to 0-30
    height *= 30;
    height = (int)height;
    
    int dither = height * opacity;

    int top = (DISPLAY_HEIGHT - height)/2;
    int bot = (DISPLAY_HEIGHT + height)/2;

    // writes from the top to the bottom
    for (i = top; i < bot; i++) {
        if (i % dither) {
            // get the chunk index and bit index of that column
            int char_index = i / BYTE_SIZE;
            int bit_index = i % BYTE_SIZE;

            // set the bit
            display[char_index][col] |= 1 << bit_index;
        }
    }
}

/* Raytrace the walls based on the player position and orientation
 * @param playerDirection: pointer to the player's direction
 * @param playerPosX:      pointer to the player's x position
 * @param playerPosY:      pointer to the player's y position
 * @param map:             pointer to the map
 * @param mapSize:         size of the map
 */
void castRay(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize, char display[4][DISPLAY_WIDTH]) {
    // Cast a ray from the player position to the edge of the screen
    // Calculate the distance to the wall
    // Calculate the opacity of the wall
    // Draw the wall
    int r, mx, my, mp, dof;
    float rayX, rayY, rayDirection, xo, yo, disT;
    rayDirection = *playerDirection - DR * DISPLAY_WIDTH/2;
    if (rayDirection < 0) {
        rayDirection += 2*PI;
    }
    if (rayDirection > 2*PI) {
        rayDirection -= 2*PI;
    }
    for (r = 0; r < DISPLAY_WIDTH; r++) {
        // Check horizontal lines
        dof = 0;
        float disH=1000000, hx=*playerPosX, hy=*playerPosY;
        float aTan = -1/tan(rayDirection);
        if (rayDirection > PI) {
            rayY = (((int)*playerPosY >> 6) << 6) - 0.0001;
            rayX = (*playerPosY - rayY) * aTan + *playerPosX;
            yo = -64;
            xo = -yo * aTan;
        }
        if (rayDirection < PI) {
            rayY = (((int)*playerPosY >> 6) << 6) + 64;
            rayX = (*playerPosY - rayY) * aTan + *playerPosX;
            yo = 64;
            xo = -yo * aTan;
        }
        if (rayDirection == 0 || rayDirection == PI) {
            rayX = *playerPosX;
            rayY = *playerPosY;
            dof = 8;
        }

        // currently stuck in infinite loop, need to fix
        while(dof < 8) {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mp = my * mapSize + mx;
            if (mp > 0 && mp < mapSize * mapSize && map[mp] != 0) {
                // hit wall
                // Calculate distance to wall
                disH = sqrt((rayX - *playerPosX) * (rayX - *playerPosX) + (rayY - *playerPosY) * (rayY - *playerPosY));
                dof = 8;
            } else {
                // next line
                rayX += xo;
                rayY += yo;
                dof += 1;
            }
        }
        
        // Check vertical lines
        dof = 0;
        float disV=1000000, vx=*playerPosX, vy=*playerPosY;
        float nTan = -tan(rayDirection);
        if (rayDirection > P2 && rayDirection < P3) {
            rayX = (((int)*playerPosX >> 6) << 6) - 0.0001;
            rayY = (*playerPosX - rayX) * nTan + *playerPosY;
            xo = -64;
            yo = -xo * nTan;
        }
        if (rayDirection < P2 || rayDirection > P3) {
            rayX = (((int)*playerPosX >> 6) << 6) + 64;
            rayY = (*playerPosX - rayX) * nTan + *playerPosY;
            xo = 64;
            yo = -xo * nTan;
        }
        if (rayDirection == 0 || rayDirection == PI) {
            rayX = *playerPosX;
            rayY = *playerPosY;
            dof = 8;
        }
        
        while (dof < 8) {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mp = my * mapSize + mx;
            if (mp > 0 && mp < mapSize * mapSize && map[mp] != 0) {
                // hit wall
                // Calculate distance to wall
                disV = sqrt((rayX - *playerPosX) * (rayX - *playerPosX) + (rayY - *playerPosY) * (rayY - *playerPosY));
                dof = 8;
            } else {
                // next line
                rayX += xo;
                rayY += yo;
                dof += 1;
            }
        }

        if(disV < disH) {rayX=vx; rayY=vy; disT=disV; }//side=0;}
        if(disH < disV) {rayX=hx; rayY=hy; disT=disH; }//side=1;}

        // Draw one line of the wall
        drawLine(display, r, disT, 1);


        rayDirection += DR;
        if (rayDirection < 0) {
            rayDirection += 2*PI;
        }
        if (rayDirection > 2*PI) {
            rayDirection -= 2*PI;
        }
    }
}

/* Move the player in the directino they are facing
 * @param playerDirection: pointer to the player's direction
 * @param playerPosX:      pointer to the player's x position
 * @param playerPosY:      pointer to the player's y position
 * @param map:             pointer to the map
 * @param mapSize:         size of the map
 */
void movePlayer(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize) {
    // determine the player movement based on the direction
    float moveX = cos(*playerDirection) + 0.5; // roundabout way of rounding to the nearest integer
    float moveY = sin(*playerDirection) + 0.5;

    // check for collisions
    if (map[(int)(*playerPosY + moveY) * mapSize + (int)(*playerPosX + moveX)] == 0) {
        *playerPosX += moveX;
        *playerPosY += moveY;
    }
}


// used to test the code, final version will be in main.c
// int main() {
//     char display[4][DISPLAY_WIDTH];
//     memset(display, 0, sizeof(display));
//     int map[] = {
//         1, 1, 1, 1, 1, 1, 1, 1,
//         1, 0, 1, 0, 0, 0, 0, 1,
//         1, 0, 1, 0, 1, 0, 0, 1,
//         1, 0, 1, 1, 1, 0, 0, 1,
//         1, 0, 0, 0, 0, 0, 0, 1,
//         1, 0, 0, 0, 0, 1, 0, 1,
//         1, 0, 0, 0, 0, 0, 0, 1,
//         1, 1, 1, 1, 1, 1, 1, 1
//     };
//     int playerPosX = 54, playerPosY = 54;
//     float playerDirection = 0;

//     // Test:  distance can't be more than DISPLAY_HEIGHT
//     // In the future, display will be a 2D array and only one column will be passed through
//     castRay(&playerDirection, &playerPosX, &playerPosY, map, 8, display);

//     // print the display
//     // 4 characters per line, 128 lines
//     for (int i = 0; i < DISPLAY_HEIGHT; i++) {
//         for (int j = 0; j < 4; j++) {
//             printf("%c", display[j][i]);
//         }
//         printf("\n");
//     }

//     return 0;
// }
