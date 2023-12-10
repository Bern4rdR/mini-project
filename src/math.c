#include "math.h"


/* Return the angle in the range 0-2PI
 * @param rad: the angle in radians
 */
float range_reduce(float rad) {
    while (rad < 0) {
        rad += 2 * PI;
    }
    while (rad > 2 * PI) {
        rad -= 2 * PI;
    }
    return rad;
}

/* Taylor approximation of the sine of the argument.
 * @param deg: the angle in degrees
*/
float sin(float rad){
    rad = range_reduce(rad);
    // Taylor series approximation
    float sin = rad;
    float term = rad;
    int i;
    for(i = 1; i < 10; i++) {
        term *= -1 * rad * rad / ((2 * i + 1) * (2 * i));
        sin += term;
    }
    return sin;
}

/* Taylor approximation of the sine of the argument.
 * @param deg: the angle in degrees
*/
float cos(float rad){
    rad = range_reduce(rad);
    // Taylor series approximation
    float cos = 1;
    float term = 1;
    int i;
    for(i = 1; i < 50; i++) {
        term *= -1 * rad * rad / ((2 * i) * (2 * i - 1));
        cos += term;
    }

    return cos;
}

/* Taylor approximation of the tangent of the argument.
 * @param deg: the angle in degrees
*/
float tan(float rad){
    return sin(rad)/cos(rad);   
}

/* Square root of the argument.
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

/* Power of the base to the exponent.
 * @param base: the base
 * @param exponent: the exponent
 */
float pow(float base, float exponent){
    float result = 1;
    int i;
    for(i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

/* Return the floor of the argument.
 * @param n: the number to floor
 */
float floor(float x){
    return (int) x;
}

/* Return the ceiling of the argument.
 * @param n: the number to ceil
 */
float ceil(float x){
    return (int) x + 1;
}