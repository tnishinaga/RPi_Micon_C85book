#include "rpi_lib/rpi.h"

#define GPFSEL1  0x20200004
#define GPSET0	 0x2020001C
#define GPCLR0	 0x20200028

int main(void){
	rpi_init();

	// GPIO16をOutputにセット
	*(volatile unsigned int *)GPFSEL1 = 0x01 << (3*6);
	// GPIO16をLにセット
	*(volatile unsigned int *)GPCLR0 = 0x01 << 16;

	while(1);

	return 0;
}