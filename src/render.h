#include <stdint.h>
#include <pic32mx.h>


uint8_t spi_send_recv(uint8_t data);
void display_image(int x, unsigned int* data);
void display_update(void);
void display_init(void);