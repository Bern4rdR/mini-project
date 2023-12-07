#include <pic32mx.h>


void user_isr(int* walking, float* playerDirection);
void set_interrupts(void);
void open_ports(void);
int getbtns(void);