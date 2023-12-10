#include <stdint.h>
#include <pic32mx.h>


uint8_t spi_send_recv(uint8_t data);


// display helper functions
void display_init(void);
void display_image(int x, unsigned int* data);
void display_update(char textbuffer[4][16]);
void render_display(char display[4][128]);

// display clensing fucntions
void clear_display(char textbuffer[4][16]);
void erase_display();

// textbuffer helper functions
void create_textbuffer(char textbuffer[4][16]);
void strcopy(char *arr, char *str);
void display_string(char textbuffer[4][16], int line, char *s);

/* Declare text buffer for display output */
extern char textbuffer[4][16];

void quicksleep(int num);

extern const uint8_t const font[];