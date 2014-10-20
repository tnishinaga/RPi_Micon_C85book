#include "rpi_lib/rpi.h"

#define GPFSEL1  0x20200004
#define GPFSEL4  0x20200010
#define GPSET1   0x20200020
#define GPCLR1   0x2020002C

#define GPLEV0	0x20200034

#define GPPUD	0x20200094
#define GPPUDCLK0	 0x20200098


int main(void){
	rpi_init();

	// GPIO10~19: Input
	*(volatile unsigned int *)GPFSEL1 = 0;
	// GPIO47をOutputにセット
	*(volatile unsigned int *)GPFSEL4 = 0x01 << (3*7);

	volatile unsigned int mask;
	volatile unsigned int val;

	mask = 0x01 << 18;

	*(volatile unsigned int *)GPPUD = 0x02;
	*(volatile unsigned int *)GPPUDCLK0 = mask;

	while(1){
		val = *(volatile unsigned int *)GPLEV0;
		if(( val & mask) != 0){
			*(volatile unsigned int *)GPCLR1 |= 0x01 << 15;
		}
		else{
			*(volatile unsigned int *)GPSET1 |= 0x01 << 15;
		}
	}

	return 0;
}