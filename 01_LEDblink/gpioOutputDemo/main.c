#include "rpi_lib/rpi.h"

#define GPFSEL4  0x20200010
#define GPSET1   0x20200020
#define GPCLR1   0x2020002C

int main(void){
	rpi_init();

	// GPIO47をOutputにセット
    *(volatile unsigned int *)GPFSEL4 = 0x01 << (3*7);
    // GPIO47をHにセット
    *(volatile unsigned int *)GPSET1 = 0x01 << 15;

	while(1);

	return 0;
}