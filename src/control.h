#include <pic32mx.h>


// Initialize the Interrupt Service Routine (ISR)
void user_isr(int* walking, float* playerDirection);

// Interrupts
void set_interrupts(void);
void open_ports(void);

// Timer
void initTimer(void);
void tick(void);

// Analog-to-digital converter (ADC), for potentiometer
void initADC(void);
int readADC(void);

// Buttons
int getbtns(void);