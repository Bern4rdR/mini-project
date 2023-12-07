#include "math.h"

/* Returns an approximation of the sine of the argument.
 * @param deg: the angle in degrees
*/
float sin(float rad){
    float deg = rad_to_deg(rad);

    // Taylor series approximation
    float sin = deg;
    float term = deg;
    int i;
    for(i = 1; i < 10; i++) {
        term *= -1 * deg * deg / ((2 * i + 1) * (2 * i));
        sin += term;
    }
    return deg_to_rad(sin);
}

/* Returns an approximation of the sine of the argument.
 * @param deg: the angle in degrees
*/
float cos(float rad){
    float deg = rad_to_deg(rad);

    // Taylor series approximation
    float cos = 1;
    float term = 1;
    int i;
    for(i = 1; i < 10; i++) {
        term *= -1 * deg * deg / ((2 * i) * (2 * i - 1));
        cos += term;
    }

    return deg_to_rad(cos);
}

float tan(float rad){
    return sin(rad)/cos(rad);   
}

float deg_to_rad(float deg){
    return deg * DR;
}

float rad_to_deg(float rad){
    return rad / DR;
}

/* Returns the square root of the argument.
 * @param n: the number to take the square root of
*/
float sqrt(float n) {
    float x = n/2;
    // int x = pow(2, ceil(numbits(n)/2));
    int i, y;
    for(i = 0; i < 50; i++) {
        int y = floor((x + floor(n/x))/2);
        if (y >= x) {
            return x;
        }
        x = y;
    }
    return x;
}

float floor(float x){
    return (int) x;
}

float ceil(float x){
    return (int) x + 1;
}

float pow(float base, float exponent){
    float result = 1;
    int i;
    for(i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}