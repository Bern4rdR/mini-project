/* 
 * This file will interact with the display on the ChipKIT Uno32 board.
 * It will take a bitmap for pixels that should be on (1) or off (0)
 * and put that signal through to the ChipKIT by activating the display.
 */
#include <pic32mx.h>


void display_image(int x, unsigned int* data) {
    for(i = 0; i < 4; i++) {
		PORTFCLR = 0X10;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		PORTFSET = 0X10;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void display_update(void) {
    int i, j, k;
    int c;
    for(i = 0; i < 4; i++) {
		PORTFCLR = 0X10;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		PORTFSET = 0X10;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}