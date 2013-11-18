#include "rpi_lib/rpi.h"


#define GPFSEL1  0x20200004
#define GPSET0	 0x2020001C
#define GPCLR0	 0x20200028

#define GPLEV0	0x20200034

#define GPPUD	0x20200094
#define GPPUDCLK0	 0x20200098


int main(void){
	rpi_init();

	// All Input
	*(volatile unsigned int *)GPFSEL1 = 0;
	// GPIO16をOutputにセット
	*(volatile unsigned int *)GPFSEL1 |= 0x01 << (3*6);

	volatile unsigned int mask;
	volatile unsigned int val;

	mask = 0x01 << 18;

	*(volatile unsigned int *)GPPUD = 0x02;
	*(volatile unsigned int *)GPPUDCLK0 = mask;

	while(1){
		val = *(volatile unsigned int *)GPLEV0;
		if(( val & mask) != 0){
			*(volatile unsigned int *)GPSET0 |= 0x01 << 16;
		}
		else{
			*(volatile unsigned int *)GPCLR0 |= 0x01 << 16;
		}
	}

	return 0;
}