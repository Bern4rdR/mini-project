#define DISPLAY_WIDTH 128

#include <pic32mx.h>


void drawLine(char* line[4], float distance, float opacity);
void castRay(float* playerDirection, int*playerPosX, int* playerPosY, int map[], int mapSize, char display[4][DISPLAY_WIDTH]);
void movePlayer(float* playerDirection, int* playerPosX, int* playerPosY, int map[], int mapSize);