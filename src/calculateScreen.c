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
#define DR 0.0174532925


#include <stdio.h>
#include <math.h>
#include <string.h>


/* Draw a vertical line with specified height and opacity
 * @param line:     pointer to the line to be drawn
 * @param distance: distance to the wall
 * @param opacity:  opacity of the line (0-1)
 */
void drawLine(char* line[4], float distance, float opacity) {
    // clear column
    memset(*line, 0, 4 * sizeof(char*));

    // calculate the height of the line
    int height = DISPLAY_HEIGHT - (int)distance;
    int dither = height * opacity;

    for (int i = distance/2; i < height; i++) {
        if (i % dither) {
            // get the chunk index and bit index of that column
            int char_index = i / BYTE_SIZE;
            int bit_index = i % BYTE_SIZE;

            // set the bit
            *line[char_index] |= 1 << bit_index;
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
    rayDirection = *playerDirection - DR;
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
        while(dof >= 8) {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mp = my * mapSize + mx;
            if (mp > 0 && mp < mapSize * mapSize && map[mp] == 1) {
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
        if (rayDirection > PI/2 && rayDirection < 3*PI/2) {
            rayX = (((int)*playerPosX >> 6) << 6) - 0.0001;
            rayY = (*playerPosX - rayX) * nTan + *playerPosY;
            xo = -64;
            yo = -xo * nTan;
        }
        if (rayDirection < PI/2 || rayDirection > 3*PI/2) {
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
            if (mp > 0 && mp < mapSize * mapSize && map[mp] == 1) {
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
        char* column[4] = {&display[0][r], &display[1][r], &display[2][r], &display[3][r]};
        drawLine(column, disT, 1);


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
    float moveX = cos(*playerDirection) * 0.1;
    float moveY = sin(*playerDirection) * 0.1;

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