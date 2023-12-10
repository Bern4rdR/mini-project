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
    float height = 1 - distance/32;
    if (height < 0) {
        height = 0;
    }
    height = (int)(height * 30);

    int dither = 1/opacity;
    int offset = (int)(distance * 10) % dither;

    int top = (DISPLAY_HEIGHT - height)/2;
    int bot = (DISPLAY_HEIGHT + height)/2;

    // writes from the top to the bottom
    for (i = top; i < bot; i++) {
        if ((i + offset) % dither == 0) {
            // calculate the index of the character in the display
            int char_index = i / BYTE_SIZE;
            // get the chunk index and bit index of that column
            int bit_index  = i % BYTE_SIZE;

            // set the bit
            display[char_index][col] |= 1 << bit_index;
        }
    }
}

/* Raycast to find the walls based on the player position and orientation
 * @param playerDirection: pointer to the player's direction
 * @param playerPosX:      pointer to the player's x position
 * @param playerPosY:      pointer to the player's y position
 * @param map:             the map
 * @param mapSize:         size of the map
 */
void castRay(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize, char display[4][DISPLAY_WIDTH]) {
    int r, mx, my, mp, dof;
    float rayX, rayY, rayDirection, xo, yo, disT;
    rayDirection = *playerDirection - DR * DISPLAY_WIDTH/2;
    rayDirection = range_reduce(rayDirection);

    // Cast rays
    for (r = (DISPLAY_WIDTH - 1); r >= 0; r++) {
        // Check horizontal lines
        dof = 0;
        float disH=1000000, hx=*playerPosX, hy=*playerPosY;
        float aTan = -1/tan(rayDirection);
        if (rayDirection > PI) {
            rayY = (((int)*playerPosY >> 3) << 3) - 0.0001;
            rayX = ((*playerPosY - rayY) * aTan) + *playerPosX;
            yo = -8;
            xo = -yo * aTan;
        }
        if (rayDirection < PI) {
            rayY = (((int)*playerPosY >> 3) << 3) + 8;
            rayX = ((*playerPosY - rayY) * aTan) + *playerPosX;
            yo = 8;
            xo = -yo * aTan;
        }
        if (rayDirection == 0 || rayDirection == PI) {
            rayX = *playerPosX;
            rayY = *playerPosY;
            dof = 8;
        }

        while(dof < 8) {
            mx = (int)(rayX) >> 3;
            my = (int)(rayY) >> 3;
            mp = (my * mapSize) + mx;
            // hit wall
            if (mp > 0 && mp < mapSize * mapSize && map[mp] == 1) {
                // Calculate distance to wall
                disH = sqrt(pow((rayX - *playerPosX), 2) + pow((rayY - *playerPosY), 2));
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
            rayX = (((int)*playerPosX >> 3) << 3) - 0.0001;
            rayY = ((*playerPosX - rayX) * nTan) + *playerPosY;
            xo = -8;
            yo = -xo * nTan;
        }
        if (rayDirection < P2 || rayDirection > P3) {
            rayX = (((int)*playerPosX >> 3) << 3) + 8;
            rayY = ((*playerPosX - rayX) * nTan) + *playerPosY;
            xo = 8;
            yo = -xo * nTan;
        }
        if (rayDirection == 0 || rayDirection == PI) {
            rayX = *playerPosX;
            rayY = *playerPosY;
            dof = 8;
        }
        
        while (dof < 8) {
            mx = (int)(rayX) >> 3;
            my = (int)(rayY) >> 3;
            mp = (my * mapSize) + mx;
            // hit wall
            if (mp > 0 && mp < mapSize * mapSize && map[mp] == 1) {
                // Calculate distance to wall
                disV = sqrt(pow((rayX - *playerPosX), 2) + pow((rayY - *playerPosY), 2));
                dof = 8;
            } else {
                // next line
                rayX += xo;
                rayY += yo;
                dof += 1;
            }
        }

        float opacity = 1.0;

        if(disV < disH) {rayX=vx; rayY=vy; disT=disV; opacity = 0.5;}//side=0;}
        if(disH < disV) {rayX=hx; rayY=hy; disT=disH;}//side=1;}

        // correct for fisheye effect
        // float ca = *playerDirection - rayDirection;
        // ca = range_reduce(ca);
        // disT = disT * cos(ca);
        // Draw one line of the wall
        drawLine(display, r, disT, opacity);


        rayDirection -= DR;
        rayDirection = range_reduce(rayDirection);
    }
}

int within_margin(float value, float target, float margin) {
    return value > target - margin && value < target + margin;
}

/* Move the player in the directino they are facing
 * @param playerDirection: pointer to the player's direction
 * @param playerPosX:      pointer to the player's x position
 * @param playerPosY:      pointer to the player's y position
 * @param map:             the map
 * @param mapSize:         size of the map
 */
void movePlayer(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize) {
    // determine the player movement based on the direction
    // down is positive, right is positive
    int moveX = 0, moveY = 0;
    float margin = PI / 8;
    if (within_margin(*playerDirection, PI/4, margin)) {
        // player is moving up and right
        moveX =  1;
        moveY =  1;
    } else if (within_margin(*playerDirection, 3*PI/4, margin)) {
        // player is moving down and right
        moveX = -1;
        moveY =  1;
    } else if (within_margin(*playerDirection, 5*PI/4, margin)) {
        // player is moving down and left
        moveX = -1;
        moveY = -1;
    } else if (within_margin(*playerDirection, 7*PI/4, margin)) {
        // player is moving up and left
        moveX =  1;
        moveY = -1;
    } else if (within_margin(*playerDirection, 0, margin)) {
        // player is moving right
        moveX =  1;
    } else if (within_margin(*playerDirection, PI, margin)) {
        // player is moving left
        moveX = -1;
    } else if (within_margin(*playerDirection, PI/2, margin)) {
        // player is moving up
        moveY =  1;
    } else if (within_margin(*playerDirection, 3*PI/2, margin)) {
        // player is moving down
        moveY = -1;
    }

    *playerPosX += moveX;
    *playerPosY += moveY;

    // check for collisions before moving
    // if (map[((int)(*playerPosY + moveY) * mapSize) + (int)(*playerPosX + moveX)] == 0) {
    //     *playerPosX += moveX;
    //     *playerPosY += moveY;
    // }
}