/*
 * This file will calculate and store the player position and orientation
 * on a map. It will also calculate the distance to the walls and their oppacity
 */
#define PI 3.14159265358979323846


void map() {
    int mapX=8, mapY=8, mapSize=64;
    int map[] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };
}

void movePlayer(float* playerDirection, int* playerPosX, int* playerPosY) {
    *playerPosX += (int)cos(*playerDirection);
    *playerPosY += (int)sin(*playerDirection);
}

void rotatePlayer(float* playerDirection, int* rotation) {
    *playerDirection += *rotation;
    *playerDirection = fmod(*playerDirection, 2*PI);
}