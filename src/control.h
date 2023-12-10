#include <pic32mx.h>


void user_isr(int* walking, float* playerDirection);
// havent used
// void set_interrupts(void);
// void open_ports(void);

// fucks up screen
// void initTimer(void);
void initADC(void);
int readADC(void);
int getbtns(void);