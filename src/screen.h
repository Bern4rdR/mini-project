#define DISPLAY_WIDTH 128

#include <pic32mx.h>
#include "math.h"


void drawLine(char display[4][128], int col, float distance);
void castRay(float* playerDirection, int*playerPosX, int* playerPosY, int map[], int mapSize, char display[4][DISPLAY_WIDTH]);
void movePlayer(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize);