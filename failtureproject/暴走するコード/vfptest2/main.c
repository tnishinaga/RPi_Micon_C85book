#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

int main(void){
	rpi_init();

	// debug
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);

	double d=10;
	uint32_t ui = 100;

	d = (d + ui) / 1000.0;

	printf("d = %lf",d);

	while(1);

	return 0;
}

void pmode(void){
	printf("CPSR: 0x%08x\n",getmode());
}