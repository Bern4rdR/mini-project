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

// game constants
#define FOV 60



/* Draw a vertical line with specified height and opacity
 * @param line:     pointer to the line to be drawn
 * @param distance: distance to the wall
 * @param opacity:  opacity of the line (0-1)
 */
void drawLine(int* line, float distance, float opacity) {
    // Clear line
    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        line[i] = 0;
    }

    // Calculate the height of the line
    int height = DISPLAY_HEIGHT - (int)distance;
    int dither = height * opacity;

    // Set the pixels on the line
    int i;
    for (i = distance/2; i < DISPLAY_HEIGHT - distance/2; i++) {
        // Use opacity to simulate color
        line[i/BYTE_SIZE] |= (i % dither) ? 1UL << i % BYTE_SIZE : 0UL << i % BYTE_SIZE;
    }
}

/* Raytrace the walls based on the player position and orientation
 * @param playerDirection: pointer to the player's direction
 * @param playerPosX:      pointer to the player's x position
 * @param playerPosY:      pointer to the player's y position
 * @param map:             pointer to the map
 * @param mapSize:         size of the map
 */
float castRay(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize, int* display[]) {
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
    for (r = 0; r < FOV; r++) {
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

        while(dof < 8) {
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
        
        while (dof = 8) {
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

        // Draw the wall
        drawLine(&display[r], disT, 1);

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


int main() {
    // Display with 1x720 resolution:  initialize all bits to 0
    unsigned char display[DISPLAY_HEIGHT/BYTE_SIZE] = {0};

    // Test:  distance can't be more than DISPLAY_HEIGHT
    // In the future, display will be a 2D array and only one column will be passed through
    drawLine(&display, 360, 50);

    return 0;
}
